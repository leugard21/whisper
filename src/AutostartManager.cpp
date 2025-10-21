#include "AutostartManager.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>
#include <qdir.h>
#include <qobject.h>
#include <qstandardpaths.h>
#include <qstringview.h>

static constexpr const char *kDesktopFileName = "whatsie.desktop";

QString AutostartManager::autostartDir() {
  QString cfg =
      QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
  if (cfg.isEmpty())
    cfg = QDir::homePath() + "/.config";
  QDir d(cfg);
  d.mkpath("autostart");
  return d.filePath("autostart");
}

QString AutostartManager::desktopPath() {
  return QDir(autostartDir()).filePath(kDesktopFileName);
}

QString AutostartManager::desktopContents() {
  const QString exec = QCoreApplication::applicationFilePath();
  const QString iconName = QStringLiteral("whatsie");

  QString text;
  QTextStream out(&text);
  out << "[Desktop Entry]\n";
  out << "Type=Application\n";
  out << "Version=1.1.0\n";
  out << "Name=Whatsie\n";
  out << "Comment=WhatsApp Web Client on Linux\n";
  out << "Exec=" << exec << "\n";
  out << "Icon=" << iconName << "\n";
  out << "Terminal=false\n";
  out << "X-GNOME-Autostart-enabled=true\n";
  return text;
}

bool AutostartManager::isEnabled() {
  QFile f(desktopPath());
  if (!f.exists())
    return false;
  if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    return false;
  const QByteArray data = f.readAll();
  f.close();

  return !data.contains("hidden=true");
}

bool AutostartManager::setEnabled(bool on) {
  const QString path = desktopPath();
  if (!on) {
    return !QFile::exists(path) || QFile::remove(path);
  }

  QFile f(path);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;
  const QString contents = desktopContents();
  const auto written = f.write(contents.toUtf8());
  f.close();
  return written == contents.toUtf8().size();
}