#!/data/data/com.termux/files/usr/bin/bash
cd "$(dirname "$0")"
chmod +x motor.biblia
grep -q "alias biblia=" ~/.bashrc 2>/dev/null || echo "alias biblia='cd "$(pwd)" && ./motor.biblia'" >> ~/.bashrc
echo ""
echo "✅ Instalação concluída! Nas próximas vezes, basta digitar: biblia"
echo ""
./motor.biblia
