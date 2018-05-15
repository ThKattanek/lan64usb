# Lan64USB
Diese Hard- und Software ermöglicht den Transfer von Programmen vom PC zum Commodore 64 (C64).

### Anleitung ist hier nur für Linux User !

## Vorraussetzungen

Wird noch ergänzt !!

## Komplettes Repositorie auf dem PC laden
```bash
cd ~
git clone https://github.com/ThKattanek/lan64usb.git
```

## Erstellen der PC Anwendung "Lan64Tool"
```bash
cd ~/lan64usb/pc-software/lan64tool
mkdir build
cd build
qmake ..
make
```
## Erstellen des Konsolentools lan64-send"
```bash
cd ~/lan64usb/pc-software/konsolen_tools/lan64_send
mkdir build
cd build
qmake ..
make
```

## AVR Firmware erstellen und flashen
```bash
cd ~/lan64usb/firmware
make hex        // Erstellt das Binary
make program    // Flasht das Binary auf den avr mit avrdude (evtl. anpassen an eueren Programmer)
make clean      // Alles wieder aufräumen
```