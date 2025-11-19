#pragma once

#include <QString>
#include <QWebEngineProfile>

class WebProfileManager {
public:
  WebProfileManager();
  QWebEngineProfile *profile() const { return m_profile; }

private:
  QWebEngineProfile *m_profile = nullptr;
  QString appDataDir() const;
  void ensureDirs(const QString &path) const;
  void applySettings(QWebEngineProfile *p);
};