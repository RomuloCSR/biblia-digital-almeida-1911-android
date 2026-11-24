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
