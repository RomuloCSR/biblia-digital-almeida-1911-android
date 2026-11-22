#!/data/data/com.termux/files/usr/bin/bash
set -e
DEST="$HOME/biblia-1911"
command -v unzip >/dev/null 2>&1 || pkg install -y unzip
mkdir -p "$DEST"
cd "$DEST"
if [ ! -f "$DEST/motor.biblia" ]; then
  echo "Baixando Biblia Almeida 1911 (Android ARM64)..."
  curl -sL https://github.com/RomuloCSR/biblia-digital-almeida-1911-android/archive/refs/heads/main.zip -o biblia.zip
  unzip -qo biblia.zip
  cp -rf biblia-digital-almeida-1911-android-main/* "$DEST/"
  rm -rf biblia-digital-almeida-1911-android-main biblia.zip
fi
chmod +x "$DEST/motor.biblia"
grep -q "alias biblia=" ~/.bashrc 2>/dev/null || echo "alias biblia='cd "$DEST" && ./motor.biblia'" >> ~/.bashrc
echo ""
echo "Pronto! Nas proximas vezes (sem internet), digite apenas: biblia"
echo ""
exec "$DEST/motor.biblia" </dev/tty
