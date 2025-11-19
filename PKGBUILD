pkgname=whisper
pkgver=1.2.0
pkgrel=1
pkgdesc="Whatsapp Web Client on Linux"
arch=('x86_64')
url="https://web.whatsapp.com"
license=('custom') 
depends=('qt6-base' 'qt6-webengine' 'hicolor-icon-theme' 'desktop-file-utils')
makedepends=('cmake' 'gcc' 'make')
provides=('whisper')
conflicts=('whisper-bin')
install="${pkgname}.install"
source=()
md5sums=()

build() {
  cmake -S "$startdir" -B "$srcdir/build" \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_INSTALL_PREFIX=/usr
  cmake --build "$srcdir/build" -j"$(nproc)"
}

package() {
  DESTDIR="$pkgdir" cmake --install "$srcdir/build"
}