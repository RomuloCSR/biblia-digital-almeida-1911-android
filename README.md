# Bíblia Digital Almeida 1911 em PT-BR — Android (Termux ARM64)

Texto bíblico baseado na edição **Almeida 1911 (Domínio Público)**, com atualização ortográfica e gramatical para o Português do Brasil realizada em **11/2026**, executado por um motor nativo em **Linguagem C (ARM64)** para **Termux**.

> **Nota editorial:** O conteúdo está em processo contínuo de atualização e pode eventualmente apresentar pequenas falhas ortográficas ou gramaticais. Colabore nos enviando seu feedback pela aba **Issues** deste repositório. Obrigado!

- **Objetivo:** Motor leve e rápido de navegação bíblica para Android (Termux), sem anúncios e 100% offline.
- **Autor:** Romulo CSR ([github.com/RomuloCSR](https://github.com/RomuloCSR))
- **Apoio espontâneo via PIX:** `21980934118`

---

## Como instalar e usar (Passo a passo rápido)

### 1. Requisitos e Dica de Leitura
- **Aplicativo:** [Termux](https://f-droid.org/packages/com.termux/) (recomendado instalar via F-Droid ou GitHub oficial).
- **Layout Responsivo (Em pé ou Deitado):** O motor se adapta automaticamente à largura da tela do celular — exibindo 2 colunas de livros na posição **retrato (em pé)** e 3 colunas centralizadas na posição **paisagem (deitado)**.
- **Ajuste de fonte:** Use o movimento de **pinça na tela (dois dedos)** para aumentar ou diminuir o tamanho das letras no Termux a qualquer momento.

### 2. Instalar no Termux (Apenas na primeira vez)
Abra o **Termux** no seu celular Android e execute os dois comandos abaixo:

**Passo A — Instale o `curl` (caso ainda não tenha instalado):**

```bash
pkg update -y && pkg install -y curl
```

**Passo B — Baixe e execute o instalador oficial:**

```bash
curl -sLO https://raw.githubusercontent.com/RomuloCSR/biblia-digital-almeida-1911-android/main/instalar.sh && bash instalar.sh
```

### 3. Abrir a Bíblia depois de instalada
Sempre que quiser abrir a Bíblia novamente no Termux (mesmo sem internet), basta digitar:

```bash
biblia
```

---

## Recursos do Motor em C

- **100% Offline:** Todos os 66 livros (Antigo e Novo Testamento) ficam salvos localmente no aparelho.
- **Atualização Ortográfica e Gramatical (PT-BR):** Conversão automática em tempo real da grafia histórica de 1911 para o português contemporâneo do Brasil.
- **Código-fonte:** leitor em `terminal.c`; o texto (limpeza do Markdown e atualização ortográfica) vem do kernel da Bíblia na pasta `kernel/` (veja `kernel/LEIAME.md`).
- **Compilar no próprio Termux (opcional):** `pkg install -y clang` e depois `clang -O2 terminal.c kernel/biblia_kernel.c -o motor.biblia`
- **Leitura Paginada:** Navegação confortável página por página (`[ENTER]`), leitura contínua do capítulo inteiro (`[T]`) ou retorno rápido ao menu (`[0]`).

---

## Este aplicativo é seguro? (Sobre alertas de segurança do sistema)

**Sim, 100% seguro.** Este é um projeto independente, gratuito e sem fins lucrativos, publicado em código aberto no **GitHub** (plataforma de referência mundial utilizada por desenvolvedores).

- **Por que alguns sistemas exibem alerta de "aplicativo não reconhecido"?** Sistemas operacionais modernos (Android, Windows, macOS e algumas distribuições Linux) emitem avisos preventivos para qualquer programa que não possua um certificado comercial pago nem esteja publicado nas lojas oficiais (Google Play, Microsoft Store ou App Store).
- **Transparência total (Código Aberto):** Todo o código-fonte deste aplicativo (`terminal.c` e a pasta `kernel/`), o script de instalação (`instalar.sh`) e os textos dos 66 livros estão abertos e visíveis nesta página. Qualquer técnico ou programador pode inspecionar o código e confirmar que não há coleta de dados, rastreamento ou arquivos maliciosos.