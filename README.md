# Bíblia Digital Almeida 1911 em PT-BR — Android ARM64 (Domínio Público)

Leitor bíblico leve, rápido e offline compilado nativamente para **Android (ARM64)** com o **Android NDK**, trazendo atualização ortográfica e gramatical realizada em **11/2026** para o português do Brasil.

## Como baixar e ler no Android (Via Termux)

1. Instale o aplicativo **Termux** no seu celular Android (preferencialmente via F-Droid ou GitHub Releases).
2. Baixe este repositório como `.zip` (botão verde **`<> Code`** -> **`Download ZIP`**) e extraia a pasta no armazenamento interno do celular (ex.: na pasta `Download`).
3. Abra o **Termux**, libere o acesso ao armazenamento e entre na pasta extraída:

```bash
termux-setup-storage
cd ~/storage/downloads/biblia-digital-almeida-1911-android-main
```

4. Dê permissão de execução ao binário ARM64 e inicie a leitura:

```bash
chmod +x motor.biblia
./motor.biblia
```

## Comandos dentro da leitura

* **`ENTER`** — Avança para a próxima página do capítulo.
* **`T` + `ENTER`** — Exibe o capítulo inteiro de uma só vez.
* **`0` + `ENTER`** — Volta imediatamente para o menu principal.
* **Pinça na tela (dois dedos)** — Aumenta ou diminui o tamanho da letra no Termux.

---

* **Autor:** Romulo CSR ([github.com/RomuloCSR](https://github.com/RomuloCSR))
* **Apoio espontâneo (PIX):** `21980934118`
