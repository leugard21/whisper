#pragma once

#include <QString>

class Settings {
public:
  static QString appDataDir();
  static QString lockFilePath();

  static bool startMinimized();
  static void setStartMinimized(bool enabled);
};