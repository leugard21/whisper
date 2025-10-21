#pragma once

#include <QString>

class AutostartManager {
public:
  static bool isEnabled();

  static bool setEnabled(bool run);

private:
  static QString autostartDir();
  static QString desktopPath();
  static QString desktopContents();
};