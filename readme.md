# Lan64USB
Diese Hard- und Software ermöglicht den Transfer von Programmen vom PC zum Commodore 64 (C64).

### Anleitung ist hier für Linux User

## Komplettes Repositorie auf dem PC laden
```bash
cd ~
git clone https://github.com/ThKattanek/lan64usb.git
```

## Erstellen der PC Anwendung "Lan64Tool"
```bash
cd ~/lan64usb/pc-software/lan64tool/
mkdir build
cd build
qmake ..
make
```
