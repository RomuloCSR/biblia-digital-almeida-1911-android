/* =============================================================================
 * PROJETO: Bíblia Digital Almeida 1911 em PT-BR (Domínio Público)
 *          Atualização Ortográfica e Gramatical realizada em 11/2026
 * OBJETIVO: Leitor de terminal para Android (Termux, ARM64). Mostra o menu,
 *           lê os capítulos (.md) e os exibe em páginas centralizadas, com
 *           layout responsivo (retrato/paisagem) e redesenho ao girar a tela.
 * TEXTO:   desde 28/11/2026 a limpeza do Markdown e a atualização ortográfica
 *          vêm do kernel da Bíblia (pasta kernel/): o mesmo texto usado nas
 *          outras versões, já com os nomes próprios e a revisão dos versículos.
 *          As regras de ortografia não ficam neste arquivo: elas são editadas
 *          no motor.biblia.c de origem (fora desta pasta) e o kernel é gerado
 *          de novo a partir dele (veja kernel/LEIAME.md).
 * AUTOR: Romulo CSR (GitHub: RomuloCSR)
 * AMBIENTE: Android ARM64 / Termux (clang) ou Android NDK
 * COMPILAR (Termux): clang -O2 terminal.c kernel/biblia_kernel.c -o motor.biblia
 * COMPILAR (NDK):    aarch64-linux-android24-clang -O2 -Wall -Wextra \
 *                    terminal.c kernel/biblia_kernel.c -o motor.biblia
 * =============================================================================
 */

#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>
#include <termios.h>

#include "kernel/biblia_kernel.h"

/* -----------------------------------------------------------------------------
 * 0. REDIMENSIONAMENTO DA TELA (SIGWINCH)
 *    O que faz: quando o celular gira (retrato/paisagem) ou a fonte muda, o
 *    Termux envia SIGWINCH; o manipulador marca a tela para redesenho.
 *    Por quê: o layout é calculado pela largura atual do terminal.
 *    Data: mantido na conversão para o kernel em 28/11/2026.
 *    Fonte: motor.biblia.c original deste repositório (commit c02b4f0).
 * ----------------------------------------------------------------------------- */

// Variaveis globais para controle de redimensionamento
volatile sig_atomic_t redesenhar_tela = 0;
int LARGURA_PAGINA_ATUAL;
int LINHAS_POR_PAGINA_ATUAL;

// Manipulador de sinal para redimensionamento de janela (SIGWINCH)
void sigwinch_handler(int signum) {
    (void)signum;
    struct winsize w;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0) {
        // Atualiza as variaveis globais com as novas dimensoes
        // Usaremos nomes ligeiramente diferentes internamente para clareza
        // Mas vamos assumir que o codigo principal usa LARGURA_PAGINA, etc.
        // Para este patch simples, vamos apenas setar uma flag de refresh.
        // O motor principal ja calcula a largura dinamicamente na maioria dos casos.
        redesenhar_tela = 1;
        // Opcional: Se o seu motor nao recalcular dinamicamente, voce teria que atualizar as globais aqui.
        // Como boa pratica, vamos assumir que ele recalcula no loop.
    }
    // Garante que o sinal nao seja ignorado e o padrao seja restaurado
    signal(SIGWINCH, sigwinch_handler);
}
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/ioctl.h>

#define MAX_LINHA 8192
#define LARGURA_PAGINA_MAX 74
#define LARGURA_PAGINA (obter_largura_pagina())

/* -----------------------------------------------------------------------------
 * CONSTANTES DE PÁGINA E CORES ANSI
 *    O que faz: limita a largura da página (máx. 74 colunas) e define cores
 *    suaves para versículos, títulos e bordas.
 *    Por quê: conforto de leitura na tela do celular.
 *    Data: mantido na conversão para o kernel em 28/11/2026.
 *    Fonte: motor.biblia.c original deste repositório.
 * ----------------------------------------------------------------------------- */

/* Códigos ANSI suaves para conforto de leitura */
#define COR_RESET   "\033[0m"
#define COR_NEGRITO "\033[1m"
#define COR_ITALICO "\033[3m"
#define COR_VERSO   "\033[1;36m"
#define COR_TITULO  "\033[1;33m"
#define COR_BORDA   "\033[36m"
#define COR_DIM     "\033[2m"

/* -----------------------------------------------------------------------------
 * 1. KERNEL DA BÍBLIA (livros, capítulos e texto)
 *    O que faz: nomes, pastas e total de capítulos vêm de
 *    bk_livro_nome/pasta/capitulos(0..65), com 0..38 = Antigo Testamento e
 *    39..65 = Novo Testamento. O texto de cada verso ou subtítulo vem de
 *    bk_verso() (limpa o Markdown e atualiza a ortografia).
 *    Por quê: as tabelas e o motor de texto antigos deste arquivo foram
 *    substituídos pelo kernel, que é o conjunto de regras revisado e o mesmo
 *    em todas as versões (conferido nos 31.104 versículos).
 *    Data: 28/11/2026.
 *    Fonte: kernel/ (biblia_kernel.h e biblia_kernel.c), gerado a partir do
 *    motor.biblia.c de origem; veja kernel/LEIAME.md.
 * ----------------------------------------------------------------------------- */

/* -----------------------------------------------------------------------------
 * 2. UTILITÁRIOS DE INTERFACE, CENTRALIZAÇÃO E ENTRADA
 *    O que faz: lê números com segurança, mede o terminal, centraliza linhas,
 *    desenha separadores, mostra data/hora e a grade de livros.
 *    Por quê: a largura da página acompanha a tela (retrato ou paisagem).
 *    Data: mantido sem mudanças na conversão para o kernel em 28/11/2026.
 *    Fonte: motor.biblia.c original deste repositório.
 * ----------------------------------------------------------------------------- */

void limpar_buffer_entrada(void) {
 int c;
 while ((c = getchar()) != '\n' && c != EOF);
}

/* Lê um número inteiro de forma segura no Termux (evita loop em EOF e limpa o \n) */
int ler_inteiro_seguro(int *destino) {
 char linha[64];
 if (fgets(linha, sizeof(linha), stdin) == NULL) {
 return -1; /* EOF / entrada fechada */
 }
 char *fim;
 long val = strtol(linha, &fim, 10);
 if (fim == linha) {
 return 0; /* Usuário digitou texto ou apenas ENTER */
 }
 *destino = (int)val;
 return 1;
}

void obter_dimensoes_terminal(int *largura, int *altura) {
 struct winsize w;
 if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == 0 && w.ws_col > 0 && w.ws_row > 0) {
 *largura = w.ws_col;
 *altura = w.ws_row;
 } else {
 *largura = 80;
 *altura = 24;
 }
}

int obter_largura_pagina(void) {
  int cols = 80, rows = 24;
  obter_dimensoes_terminal(&cols, &rows);
  int larg = cols - 4;
  if (larg > LARGURA_PAGINA_MAX) larg = LARGURA_PAGINA_MAX;
  if (larg < 32) larg = 32;
  return larg;
}

int calcular_margem_esquerda(int largura_conteudo) {
 int cols = 80, rows = 24;
 obter_dimensoes_terminal(&cols, &rows);
 if (cols <= largura_conteudo) return 2;
 return (cols - largura_conteudo) / 2;
}

void imprimir_espacos(int qtd) {
 for (int i = 0; i < qtd; i++) {
 putchar(' ');
 }
}

int comprimento_visivel_utf8(const char *texto) {
 int visivel = 0;
 for (int i = 0; texto[i] != '\0'; i++) {
 if ((texto[i] & 0xC0) != 0x80) {
 visivel++;
 }
 }
 return visivel;
}

void imprimir_linha_centralizada(const char *texto, int largura_pagina) {
 int visivel = comprimento_visivel_utf8(texto);
 int margem = calcular_margem_esquerda(largura_pagina);
 int recuo_interno = (largura_pagina > visivel) ? (largura_pagina - visivel) / 2 : 0;
 imprimir_espacos(margem + recuo_interno);
 printf("%s\n", texto);
}

void imprimir_separador(int largura_pagina, char simbolo) {
 int margem = calcular_margem_esquerda(largura_pagina);
 imprimir_espacos(margem);
 printf("%s", COR_BORDA);
 for (int i = 0; i < largura_pagina; i++) {
 putchar(simbolo);
 }
 printf("%s\n", COR_RESET);
}

void pausar_tela(void) {
 int margem = calcular_margem_esquerda(LARGURA_PAGINA);
 printf("\n");
 imprimir_espacos(margem);
 printf("%s[Pressione ENTER para voltar ao menu...]%s", COR_DIM, COR_RESET);
 fflush(stdout);
 char tmp[32];
 if (fgets(tmp, sizeof(tmp), stdin) == NULL) {
 return;
 }
}

void exibir_data_hora(void) {
 time_t t = time(NULL);
 struct tm tm = *localtime(&t);

 const char *dias_semana[] = {
 "Domingo", "Segunda-feira", "Terça-feira", "Quarta-feira",
 "Quinta-feira", "Sexta-feira", "Sábado"
 };
 const char *meses[] = {
 "Jan", "Fev", "Mar", "Abr", "Mai", "Jun",
 "Jul", "Ago", "Set", "Out", "Nov", "Dez"
 };

 char cabecalho[128];
 snprintf(cabecalho, sizeof(cabecalho), "%s, %02d de %s de %d  |  %02d:%02d",
 dias_semana[tm.tm_wday],
 tm.tm_mday,
 meses[tm.tm_mon],
 tm.tm_year + 1900,
 tm.tm_hour,
 tm.tm_min);

 printf("%s", COR_DIM);
 imprimir_linha_centralizada(cabecalho, LARGURA_PAGINA);
 printf("%s", COR_RESET);
}

void imprimir_coluna(int numero, const char *texto) {
 int tamanho_visivel = comprimento_visivel_utf8(texto);

 printf(" %2d - %s", numero, texto);

 int espacos = 20 - tamanho_visivel;
 for (int j = 0; j < espacos; j++) {
 printf(" ");
 }
}


/* -----------------------------------------------------------------------------
 * 3. LEITURA E EXIBIÇÃO DE CAPÍTULOS (.md) ESTILO PÁGINA CENTRALIZADA
 *    O que faz: abre o capítulo (NN.md ou NNN.md), pagina pela altura da tela
 *    ([ENTER], [T] capítulo inteiro, [0] voltar) e quebra os versos por
 *    palavra na largura da página.
 *    Por quê: leitura confortável em qualquer posição do celular.
 *    Data: 28/11/2026 - o texto de cada linha passa pelo kernel (bk_verso);
 *    se o kernel recusar a linha (erro), ela é ignorada.
 *    Fonte: motor.biblia.c original deste repositório + kernel/ (bk_verso).
 * ----------------------------------------------------------------------------- */

FILE *abrir_arquivo_capitulo(const char *pasta_livro, int capitulo, char *caminho_out) {
 sprintf(caminho_out, "%s/%02d.md", pasta_livro, capitulo);
 FILE *f = fopen(caminho_out, "r");

 if (f == NULL) {
 sprintf(caminho_out, "%s/%03d.md", pasta_livro, capitulo);
 f = fopen(caminho_out, "r");
 }
 return f;
}

/* Retorna: 1 = continuar | 0 = voltar ('0') | 2 = reimprimir capítulo inteiro ('T') */
int verificar_pausa_pagina(int *linhas_na_pagina, int *modo_continuo, const char *nome_livro, int capitulo) {
 if (*modo_continuo) return 1;

 int cols = 80, rows = 24;
 obter_dimensoes_terminal(&cols, &rows);
 int limite_linhas = (rows > 8) ? (rows - 5) : 18;

 if (*linhas_na_pagina >= limite_linhas) {
 int margem = calcular_margem_esquerda(LARGURA_PAGINA);
 imprimir_separador(LARGURA_PAGINA, '-');
 imprimir_espacos(margem);
 printf("%s[ENTER] Próxima página  |  [T] Ler capítulo inteiro  |  [0] Voltar: %s",
 COR_DIM, COR_RESET);
 fflush(stdout);

 char resp[32];
 if (fgets(resp, sizeof(resp), stdin) == NULL) {
 return 0;
 }
 if (resp[0] == '0') {
 return 0;
 }
 if (resp[0] == 't' || resp[0] == 'T') {
 *modo_continuo = 1;
 return 2;
 }

 system("clear");
 char cabecalho[160];
 snprintf(cabecalho, sizeof(cabecalho), "%s — Capítulo %d (continuação)", nome_livro, capitulo);
 imprimir_separador(LARGURA_PAGINA, '=');
 printf("%s", COR_TITULO);
 imprimir_linha_centralizada(cabecalho, LARGURA_PAGINA);
 printf("%s", COR_RESET);
 imprimir_separador(LARGURA_PAGINA, '=');
 printf("\n");
 *linhas_na_pagina = 4;
 }
 return 1;
}

void imprimir_verso_formatado(const char *versiculo_num, const char *texto, int *linhas_impressas) {
 int margem = calcular_margem_esquerda(LARGURA_PAGINA);
 char prefixo[32];
 snprintf(prefixo, sizeof(prefixo), "[%s] ", versiculo_num);
 int larg_prefixo = comprimento_visivel_utf8(prefixo);

 imprimir_espacos(margem);
 printf("%s%s%s", COR_VERSO, prefixo, COR_RESET);

 int col_atual = larg_prefixo;
 const char *p = texto;

 while (*p != '\0') {
 while (*p == ' ') p++;
 if (*p == '\0') break;

 const char *fim_palavra = p;
 int larg_palavra = 0;
 while (*fim_palavra != '\0' && *fim_palavra != ' ') {
 if ((*fim_palavra & 0xC0) != 0x80) {
 larg_palavra++;
 }
 fim_palavra++;
 }

 if (col_atual > larg_prefixo && (col_atual + 1 + larg_palavra) > LARGURA_PAGINA) {
 putchar('\n');
 (*linhas_impressas)++;
 imprimir_espacos(margem + larg_prefixo);
 col_atual = larg_prefixo;
 } else if (col_atual > larg_prefixo) {
 putchar(' ');
 col_atual++;
 }

 fwrite(p, 1, (size_t)(fim_palavra - p), stdout);
 col_atual += larg_palavra;
 p = fim_palavra;
 }

 printf("\n\n");
 (*linhas_impressas) += 2;
}

void imprimir_subtitulo_formatado(const char *texto, int *linhas_impressas) {
 int margem = calcular_margem_esquerda(LARGURA_PAGINA);
 imprimir_espacos(margem);
 printf("%s%s(%s)%s\n\n", COR_TITULO, COR_ITALICO, texto, COR_RESET);
 (*linhas_impressas) += 2;
}

/* Retorna 1 se leu até o fim (pede ENTER no final), ou 0 se usuário apertou '0' (volta direto) */
int ler_capitulo(const char *nome_livro, const char *pasta_livro, int capitulo) {
 char caminho_ficheiro[200];
 FILE *ficheiro = abrir_arquivo_capitulo(pasta_livro, capitulo, caminho_ficheiro);

 if (ficheiro == NULL) {
 int margem = calcular_margem_esquerda(LARGURA_PAGINA);
 printf("\n");
 imprimir_espacos(margem);
 printf("(Erro: Não foi possível encontrar o arquivo '%s'.)\n", caminho_ficheiro);
 return 1;
 }

 int modo_continuo = 0;

reiniciar_leitura:
 rewind(ficheiro);
 system("clear");

 char cabecalho[160];
 snprintf(cabecalho, sizeof(cabecalho), "%s — Capítulo %d", nome_livro, capitulo);

 imprimir_separador(LARGURA_PAGINA, '=');
 printf("%s", COR_TITULO);
 imprimir_linha_centralizada(cabecalho, LARGURA_PAGINA);
 printf("%s", COR_RESET);
 imprimir_separador(LARGURA_PAGINA, '=');
 printf("\n");

 char linha[MAX_LINHA];
 char texto_limpo[MAX_LINHA];
 int linhas_na_pagina = 4;

 while (fgets(linha, sizeof(linha), ficheiro) != NULL) {
 if (strncmp(linha, "---", 3) == 0) {
 break;
 }

 if (linha[0] == '#' || linha[0] == '[') {
 continue;
 }

 int idx = 0;
 while (linha[idx] == '*' || linha[idx] == ' ') {
 idx++;
 }

 if (linha[idx] >= '1' && linha[idx] <= '9') {
 int acao = verificar_pausa_pagina(&linhas_na_pagina, &modo_continuo, nome_livro, capitulo);
 if (acao == 0) {
 fclose(ficheiro);
 return 0;
 }
 if (acao == 2) {
 goto reiniciar_leitura;
 }

 char versiculo_num[20];
 int v_pos = 0;

 while (linha[idx] >= '0' && linha[idx] <= '9' && v_pos < 19) {
 versiculo_num[v_pos++] = linha[idx++];
 }
 versiculo_num[v_pos] = '\0';

 while (linha[idx] == '*' || linha[idx] == ' ') {
 idx++;
 }

 /* Kernel: limpa o Markdown e atualiza a ortografia (linha longa demais fica de fora) */
 if (bk_verso(&linha[idx], texto_limpo, sizeof(texto_limpo)) < 0) continue;
 imprimir_verso_formatado(versiculo_num, texto_limpo, &linhas_na_pagina);
 }
 else if (strncmp(linha, "**", 2) == 0 && linha[idx] != '\0' && linha[idx] != '\n') {
 int acao = verificar_pausa_pagina(&linhas_na_pagina, &modo_continuo, nome_livro, capitulo);
 if (acao == 0) {
 fclose(ficheiro);
 return 0;
 }
 if (acao == 2) {
 goto reiniciar_leitura;
 }

 /* Kernel: limpa o Markdown e atualiza a ortografia (linha longa demais fica de fora) */
 if (bk_verso(&linha[idx], texto_limpo, sizeof(texto_limpo)) < 0) continue;
 imprimir_subtitulo_formatado(texto_limpo, &linhas_na_pagina);
 }
 }

 imprimir_separador(LARGURA_PAGINA, '=');
 fclose(ficheiro);
 return 1;
}

/* -----------------------------------------------------------------------------
 * 4. FUNÇÃO PRINCIPAL: main
 *    O que faz: registra o SIGWINCH, mostra o cabeçalho e o menu, a lista de
 *    livros e pede o capítulo. Livros vêm do kernel: base 0 = Antigo
 *    Testamento (39 livros), base 39 = Novo Testamento (27 livros).
 *    O menu mostra a linha de doação espontânea via PIX; em tela estreita
 *    (retrato) ela é dividida em duas linhas para não quebrar no meio.
 *    Por quê: uma única fonte para nomes, pastas e capítulos (o kernel).
 *    Data: 28/11/2026.
 *    Fonte: motor.biblia.c original deste repositório + kernel/.
 * ----------------------------------------------------------------------------- */

int main(void) {
    // REGISTRA O MANIPULADOR DE SINAL PARA REDIMENSIONAMENTO (SIGWINCH)
    struct sigaction sa_winch;
    memset(&sa_winch, 0, sizeof(sa_winch));
    sa_winch.sa_handler = sigwinch_handler;
    sigemptyset(&sa_winch.sa_mask);
    sa_winch.sa_flags = SA_RESTART; // Reinicia chamadas de sistema interrompidas
    if (sigaction(SIGWINCH, &sa_winch, NULL) == -1) {
        perror("Erro ao registrar SIGWINCH");
    }
    // Inicializa as variaveis globais de tamanho (para seguranca)
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    LARGURA_PAGINA_ATUAL = w.ws_col - 2; // -2 para margem
    LINHAS_POR_PAGINA_ATUAL = w.ws_row - 4; // -4 para cabecalho/rodape


 int opcao_principal = 0;
 int opcao_livro = 0;
 int opcao_capitulo = 0;

 do {
 system("clear");
 exibir_data_hora();

 int margem = calcular_margem_esquerda(LARGURA_PAGINA);

 imprimir_separador(LARGURA_PAGINA, '=');
 printf("%s", COR_TITULO);
 imprimir_linha_centralizada("BÍBLIA DIGITAL ALMEIDA 1911 EM PT-BR (DOMÍNIO PÚBLICO)", LARGURA_PAGINA);
 imprimir_linha_centralizada("Atualização Ortográfica e Gramatical realizada em 11/2026", LARGURA_PAGINA);
 printf("%s%s", COR_RESET, COR_DIM);
 imprimir_linha_centralizada("Motor C (ARM64) — Termux", LARGURA_PAGINA);
 imprimir_linha_centralizada("Autor: Romulo CSR", LARGURA_PAGINA);
 imprimir_linha_centralizada("Download free: github.com/RomuloCSR", LARGURA_PAGINA);
 if (LARGURA_PAGINA >= 50) {
   imprimir_linha_centralizada("Doação espontânea para a obra via PIX: 21980934118", LARGURA_PAGINA);
 } else {
   /* Tela estreita (retrato): divide a linha para não quebrar no meio */
   imprimir_linha_centralizada("Doação espontânea para a obra", LARGURA_PAGINA);
   imprimir_linha_centralizada("via PIX: 21980934118", LARGURA_PAGINA);
 }
 printf("%s", COR_RESET);
 imprimir_separador(LARGURA_PAGINA, '=');
 printf("\n");

 imprimir_espacos(margem + 4); printf("1 - Antigo Testamento\n");
 imprimir_espacos(margem + 4); printf("2 - Novo Testamento\n");
 imprimir_espacos(margem + 4); printf("3 - Sair\n\n");
 imprimir_espacos(margem + 4); printf("%sZoom da fonte: movimento de pinça na tela (dois dedos)%s\n\n", COR_DIM, COR_RESET);
 imprimir_espacos(margem + 4); printf("Escolha uma opção: ");
 fflush(stdout);

 int status_menu = ler_inteiro_seguro(&opcao_principal);
 if (status_menu == -1) {
 break; /* Evita loop infinito caso stdin seja fechado no Termux */
 }
 if (status_menu == 0) {
 opcao_principal = 0;
 continue;
 }

 if (opcao_principal == 1 || opcao_principal == 2) {
 /* Livros do kernel: AT = 0..38, NT = 39..65 */
 int base = (opcao_principal == 1) ? 0 : 39;
 int total_livros = (opcao_principal == 1) ? 39 : 27;

 system("clear");
 imprimir_separador(LARGURA_PAGINA, '=');
 printf("%s", COR_TITULO);
 imprimir_linha_centralizada((opcao_principal == 1) ? "ANTIGO TESTAMENTO" : "NOVO TESTAMENTO", LARGURA_PAGINA);
 printf("%s", COR_RESET);
 imprimir_separador(LARGURA_PAGINA, '=');
 printf("\n");

 int cols_grade = (LARGURA_PAGINA >= 72) ? 3 : ((LARGURA_PAGINA >= 46) ? 2 : 1);
 for (int i = 0; i < total_livros; i++) {
   if (i % cols_grade == 0) {
     imprimir_espacos(margem);
   }
   imprimir_coluna(i + 1, bk_livro_nome(base + i));
   if ((i + 1) % cols_grade == 0) {
     printf("\n");
   }
 }

 if (total_livros % cols_grade != 0) {
   printf("\n");
 }

 printf("\n");
 imprimir_espacos(margem); printf(" 0 - Voltar\n");
 imprimir_espacos(margem); printf(" Escolha um livro: ");
 fflush(stdout);

 int status_livro = ler_inteiro_seguro(&opcao_livro);
 if (status_livro == -1) {
 break;
 }
 if (status_livro == 0) {
 continue;
 }

 if (opcao_livro >= 1 && opcao_livro <= total_livros) {
 int total_do_livro = bk_livro_capitulos(base + opcao_livro - 1);

 printf("\n");
 imprimir_espacos(margem);
 printf("%s selecionado. Digite o capítulo (1 a %d): ",
 bk_livro_nome(base + opcao_livro - 1), total_do_livro);
 fflush(stdout);

 int status_cap = ler_inteiro_seguro(&opcao_capitulo);
 if (status_cap == -1) {
 break;
 }

 if (status_cap == 1 &&
 opcao_capitulo >= 1 && opcao_capitulo <= total_do_livro) {
 int leu_ate_o_fim = ler_capitulo(bk_livro_nome(base + opcao_livro - 1), bk_livro_pasta(base + opcao_livro - 1), opcao_capitulo);
 if (leu_ate_o_fim) {
 pausar_tela();
 }
 } else {
 printf("\n");
 imprimir_espacos(margem);
 printf("Capítulo inválido! O livro de %s possui capítulos de 1 a %d.\n",
 bk_livro_nome(base + opcao_livro - 1), total_do_livro);
 pausar_tela();
 }
 } else if (opcao_livro != 0) {
 printf("\n");
 imprimir_espacos(margem);
 printf("Opção de livro inválida!\n");
 pausar_tela();
 }

 } else if (opcao_principal == 3) {
 int margem_saida = calcular_margem_esquerda(LARGURA_PAGINA);
 printf("\n");
 imprimir_espacos(margem_saida);
 printf("Saindo do aplicativo...\n");
 } else {
 int margem_inv = calcular_margem_esquerda(LARGURA_PAGINA);
 printf("\n");
 imprimir_espacos(margem_inv);
 printf("Opção inválida! Tente novamente.\n");
 pausar_tela();
 }

 } while (opcao_principal != 3);

 return 0;
}
