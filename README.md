# Bíblia Digital Almeida 1911 (Android — Leitura Offline)

Leitor bíblico leve, rápido e **100% offline** para celulares Android, com atualização ortográfica e gramatical realizada em **11/2026** para o português do Brasil.

---

## Como instalar e ler em 3 passos simples

### Passo 1: Instale o aplicativo Termux
Baixe e instale o aplicativo gratuito **Termux** no seu celular Android (pelo **F-Droid** ou **GitHub Releases**).

### Passo 2: Copie e cole este comando único
Abra o **Termux**, toque na tela preta, cole o comando abaixo e aperte **Enter** (se aparecer alguma pergunta `[Y/n]`, apenas aperte **Enter**):

```bash
pkg update -y && pkg install -y wget unzip && cd ~ && rm -rf biblia-1911 biblia.zip && wget -q --show-progress [https://github.com/RomuloCSR/biblia-digital-almeida-1911-android/archive/refs/heads/main.zip](https://github.com/RomuloCSR/biblia-digital-almeida-1911-android/archive/refs/heads/main.zip) -O biblia.zip && unzip -q biblia.zip && mv biblia-digital-almeida-1911-android-main biblia-1911 && rm biblia.zip && bash ~/biblia-1911/instalar.sh
```

Pronto! A Bíblia vai abrir imediatamente na tela do seu celular.

### Passo 3: Como abrir nos próximos dias (Sem internet)
Sempre que fechar o Termux e quiser ler novamente, basta abrir o aplicativo, digitar uma única palavra e apertar **Enter**:

```bash
biblia
```

---

## Comandos dentro da leitura

* **`ENTER`** — Avança para a próxima página do capítulo.
* **`T` + `ENTER`** — Exibe o capítulo inteiro de uma só vez.
* **`0` + `ENTER`** — Volta imediatamente para o menu principal.
* **Pinça na tela (dois dedos)** — Aumenta ou diminui o tamanho da letra.

---

* **Autor:** Romulo CSR ([github.com/RomuloCSR](https://github.com/RomuloCSR))
* **Apoio espontâneo (PIX):** `21980934118`
