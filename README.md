# 📖 Bíblia Digital Almeida 1911 (PT-BR) — Android (Termux ARM64)

Motor de navegação offline desenvolvido em **Linguagem C** nativa para dispositivos **Android (ARM64)** via **Termux**, com atualização ortográfica e gramatical completa para o Português do Brasil (PT-BR) e paginação centralizada.

---

## 📱 Requisitos e Dica de Leitura

* **Aplicativo:** [Termux](https://f-droid.org/packages/com.termux/) (recomendado baixar via F-Droid ou GitHub oficial).
* **Melhor experiência visual:** Utilize o celular na posição **paisagem (horizontal)** para que as 3 colunas de livros e as margens de 74 caracteres fiquem perfeitamente centralizadas na tela.
* **Ajuste de fonte:** Use o movimento de **pinça na tela (dois dedos)** para aumentar ou diminuir o tamanho das letras no Termux a qualquer momento.

---

## 🚀 Instalação Rápida no Termux

Abra o **Termux** no seu celular Android e siga os dois passos abaixo:

### 1. Instale o curl (caso ainda não tenha instalado)
```bash
pkg update -y && pkg install -y curl
```

### 2. Baixe e execute o instalador oficial
```bash
curl -sLO [https://raw.githubusercontent.com/RomuloCSR/biblia-digital-almeida-1911-android/main/instalar.sh](https://raw.githubusercontent.com/RomuloCSR/biblia-digital-almeida-1911-android/main/instalar.sh) && bash instalar.sh
```

---

## ⚡ Como abrir depois de instalado

Sempre que quiser abrir a Bíblia novamente no Termux (mesmo sem internet), basta digitar:

```bash
biblia
```

---

## ✨ Recursos do Motor em C

* **100% Offline:** Todos os 66 livros (Antigo e Novo Testamento) ficam salvos localmente no aparelho.
* **Atualização Ortográfica e Gramatical (PT-BR):** Conversão automática em tempo real da grafia histórica de 1911 para o português contemporâneo do Brasil.
* **Leitura Paginada:** Navegação confortável página por página (`[ENTER]`), leitura contínua do capítulo inteiro (`[T]`) ou retorno rápido ao menu (`[0]`).

---

## 🤝 Apoie esta Obra

* **Autor:** Romulo CSR ([github.com/RomuloCSR](https://github.com/RomuloCSR))
* **Contribuição espontânea via PIX:** `21980934118`
* **Licença do texto bíblico:** Domínio Público (Almeida Revista e Corrigida 1911).
