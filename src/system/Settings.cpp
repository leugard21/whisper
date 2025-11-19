#include "Settings.h"

#include <QDir>
#include <QSettings>
#include <QStandardPaths>

QString Settings::appDataDir() {
  QString dir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  if (dir.isEmpty())
    dir = QDir::homePath() + "/.whisper";

  QDir(dir).mkpath(".");
  return dir;
}

QString Settings::lockFilePath() {
  return QDir(appDataDir()).filePath("whisper.lock");
}

bool Settings::startMinimized() {
  QSettings s;
  return s.value("ui/startMinimized", false).toBool();
}

void Settings::setStartMinimized(bool enabled) {
  QSettings s;
  s.setValue("ui/startMinimized", enabled);
}