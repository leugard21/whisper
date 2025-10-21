# Whatsie

Whatsie is a native desktop client for linux built with **C++** and **Qt 6**, wrapping the official [web.whatsapp.com](https://web.whatsapp.com) interface.  
It provides a fast, clean, and persistent WhatsApp experience that integrates smoothly with Linux desktop environments.

## Features

- Persistent login session using `QWebEngineProfile`
- System tray integration with unread badge and minimize-to-tray
- Desktop notifications through WhatsApp Web
- File download dialog and location control
- Keyboard shortcuts
  - **Ctrl + R** → Reload
  - **Alt + ← / Alt + →** → Back / Forward
  - **Ctrl + Shift + I** → Toggle DevTools
- External links open in your default browser
- Single-instance behavior using `QLockFile`

## Build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
./build/whatsie
```

## Install

### Arch Linux

```bash
makepkg -sf
sudo pacman -U whatsie-*.pkg.tar.zst
```

### Debian / Ubuntu

```bash
cpack -G DEB -B build
sudo dpkg -i build/whatsie_*.deb
sudo apt -f install
```

## Uninstall

```bash
sudo pacman -Rns whatsie
```
