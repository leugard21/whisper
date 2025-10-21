#include "Settings.h"
#include <QDir>
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
