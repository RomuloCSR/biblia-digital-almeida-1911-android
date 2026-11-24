#!/data/data/com.termux/files/usr/bin/bash
set -e
DEST="$HOME/biblia-1911"
command -v unzip >/dev/null 2>&1 || pkg install -y unzip
mkdir -p "$DEST"
cd "$DEST"
echo "Baixando/Atualizando Biblia Almeida 1911 (Android ARM64)..."
curl -sL https://github.com/RomuloCSR/biblia-digital-almeida-1911-android/archive/refs/heads/main.zip -o biblia.zip
unzip -qo biblia.zip
cp -rf biblia-digital-almeida-1911-android-main/* "$DEST/"
rm -rf biblia-digital-almeida-1911-android-main biblia.zip "$DEST/motor.biblia.o"
chmod +x "$DEST/motor.biblia"
grep -q "alias biblia=" ~/.bashrc 2>/dev/null || echo "alias biblia='cd \"$DEST\" && ./motor.biblia'" >> ~/.bashrc
mkdir -p "$HOME/.termux/task"
echo -e "#!/data/data/com.termux/files/usr/bin/bash\ncd \"$DEST\" && ./motor.biblia" > "$HOME/.termux/task/Bíblia"
chmod +x "$HOME/.termux/task/Bíblia"
echo ""
echo "✅ Pronto! Para ler, digite: biblia"
echo "📱 Para criar o ícone na tela inicial, adicione o widget do Termux:Widget e escolha Bíblia!"
echo ""
exec "$DEST/motor.biblia" </dev/tty
