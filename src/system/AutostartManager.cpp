#include "AutostartManager.h"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QTextStream>

static constexpr const char *kDesktopFileName = "whisper.desktop";

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

  QString text;
  QTextStream out(&text);

  out << "[Desktop Entry]\n";
  out << "Type=Application\n";
  out << "Version=" WHISPER_VERSION "\n";
  out << "Name=Whisper\n";
  out << "Comment=WhatsApp Web Client on Linux\n";
  out << "Exec=" << exec << "\n";
  out << "Icon=whisper\n";
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

bool AutostartManager::setEnabled(bool enabled) {
  const QString path = desktopPath();

  if (!enabled)
    return !QFile::exists(path) || QFile::remove(path);

  QFile f(path);
  if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    return false;

  const QString contents = desktopContents();
  const qint64 written = f.write(contents.toUtf8());
  f.close();

  return written == contents.toUtf8().size();
}
