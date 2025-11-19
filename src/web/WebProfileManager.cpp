#include "WebProfileManager.h"
#include "../system/Settings.h"

#include <QDir>
#include <QWebEngineSettings>

static constexpr const char *kUserAgent =
    "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 "
    "(KHTML, like Gecko) Chrome/140.0.0.0 Safari/537.36";

WebProfileManager::WebProfileManager() {
  const QString base = Settings::appDataDir();

  m_profile = new QWebEngineProfile(QStringLiteral("whisper_profile"));
  m_profile->setPersistentCookiesPolicy(
      QWebEngineProfile::ForcePersistentCookies);
  m_profile->setPersistentStoragePath(base + "/storage");
  m_profile->setCachePath(base + "/cache");
  m_profile->setDownloadPath(QDir::homePath() + "/Downloads");
  m_profile->setHttpUserAgent(QString::fromLatin1(kUserAgent));

  auto *s = m_profile->settings();
  s->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
  s->setAttribute(QWebEngineSettings::LocalStorageEnabled, true);
  s->setAttribute(QWebEngineSettings::AutoLoadImages, true);
  s->setAttribute(QWebEngineSettings::ScreenCaptureEnabled, true);

  QDir(base).mkpath("storage");
  QDir(base).mkpath("cache");
}
