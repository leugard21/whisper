#pragma once

#include <QString>

class Settings {
public:
  static QString appDataDir();
  static QString lockFilePath();
};