# Whisper

Whisper is a native desktop client for **WhatsApp Web**, built with **C++** and **Qt 6**. It wraps the official [web.whatsapp.com](https://web.whatsapp.com) interface to provide a seamless, persistent, and integrated experience on Linux desktops.

## Features

- **Native Integration**: Built with Qt 6 for a fast and responsive native feel.
- **System Tray Support**: Minimize to tray to keep WhatsApp running in the background.
- **Desktop Notifications**: Native notification integration for new messages.
- **Privacy & Permissions**: Granular control over camera and microphone permissions.
- **Download Manager**: Integrated file downloader.
- **Autostart**: Option to launch Whisper automatically on system startup.
- **Theme Support**: Respects system dark/light mode preferences.

## Installation

### Arch Linux

You can install Whisper from the AUR or build it manually using `makepkg`.

```bash
# Build and install package
makepkg -si
```

### Debian / Ubuntu

1.  Build the package (see [Build from Source](#build-from-source)).
2.  Install the generated `.deb` file:

```bash
sudo dpkg -i build/whisper_*.deb
sudo apt -f install  # Fix dependencies if needed
```

### Fedora / RHEL / openSUSE

1.  Build the package (see [Build from Source](#build-from-source)).
2.  Install the generated `.rpm` file:

```bash
# Fedora / RHEL
sudo dnf install ./build/whisper-*.x86_64.rpm

# openSUSE
sudo zypper install ./build/whisper-*.x86_64.rpm
```

## Build from Source

### Prerequisites

Ensure you have the following dependencies installed:

-   **C++ Compiler**: GCC or Clang (supporting C++20)
-   **CMake**: Version 3.22 or higher
-   **Qt 6**: Core, Gui, Widgets, WebEngineCore, WebEngineWidgets

### Build Steps

```bash
# 1. Clone the repository
git clone https://github.com/leugard/whisper.git
cd whisper

# 2. Configure the project
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release

# 3. Build the application
cmake --build build -j$(nproc)

# 4. Run Whisper
./build/whisper
```

## License

This project is distributed under a Custom license. See `PKGBUILD` for details.

## Credits

Maintained by **Islahul Kafaa** (Luna).
