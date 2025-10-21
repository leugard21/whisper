#include "WebProfileManager.h"
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QWebEngineSettings>

static inline QString modernDesktopUA() {
  return QStringLiteral("Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
                        "(KHTML, like Gecko) Chrome/140.0.0.0 Safari/537.36");
}

WebProfileManager::WebProfileManager() {
  const QString base = appDataDir();
  ensureDirs(base);

  m_profile = new QWebEngineProfile(QStringLiteral("whatsie_profile"));
  m_profile->setPersistentCookiesPolicy(
      QWebEngineProfile::ForcePersistentCookies);
  m_profile->setPersistentStoragePath(base + "/storage");
  m_profile->setCachePath(base + "/cache");
  m_profile->setDownloadPath(QDir::homePath() + "/Downloads");
  m_profile->setHttpUserAgent(modernDesktopUA());

  applySettings(m_profile);
}

QString WebProfileManager::appDataDir() const {
  const QString dir =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  return dir.isEmpty() ? QDir::homePath() + "/.whatsie" : dir;
}

void WebProfileManager::ensureDirs(const QString &path) const {
  QDir(path).mkpath(".");
  QDir(path).mkpath("storage");
  QDir(path).mkpath("cache");
}

void WebProfileManager::applySettings(QWebEngineProfile *p) {
  auto *s = p->settings();
  s->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
  s->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
  s->setAttribute(QWebEngineSettings::AutoLoadImages, true);
  s->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);
}