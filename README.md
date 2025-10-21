# Whisper

Whisper is a native desktop client for linux built with **C++** and **Qt 6**, wrapping the official [web.whatsapp.com](https://web.whatsapp.com) interface.  
It provides a fast, clean, and persistent WhatsApp experience that integrates smoothly with Linux desktop environments.

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/whisper
```

## Install

### Arch Linux

```bash
makepkg -sf
sudo pacman -U whisper-*.pkg.tar.zst
```

### Debian / Ubuntu

```bash
cpack -G DEB -B build
sudo dpkg -i build/whisper_*.deb
sudo apt -f install
```

## Uninstall

```bash
sudo pacman -Rns whisper
```
