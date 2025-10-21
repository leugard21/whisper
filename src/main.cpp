#include "AppWindow.h"
#include "ClientPage.h"
#include "Downloader.h"
#include "PermissionHandler.h"
#include "Settings.h"
#include "SingleInstance.h"
#include "WebProfileManager.h"
#include <QApplication>
#include <QMessageBox>
#include <QScreen>
#include <QWebEngineView>

int main(int argc, char *argv[]) {
  QApplication::setApplicationName("Whisper");
  QApplication::setOrganizationName("Luna");
  QApplication::setApplicationVersion("0.1.0");
  QApplication app(argc, argv);

  SingleInstance instance(Settings::lockFilePath());
  if (!instance.tryLock()) {
    QMessageBox::information(nullptr, "Whisper", "Whisper is already running.");
    return 0;
  }

  WebProfileManager profileMgr;
  auto *profile = profileMgr.profile();

  auto *page = new ClientPage(profile);

  PermissionHandler perms;
#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
  QObject::connect(
      page, &QWebEnginePage::permissionRequested, &perms,
      [&perms](QWebEnginePermission p) { perms.handlePermission(p); });
#else
  QObject::connect(
      page, &QWebEnginePage::featurePermissionRequested, &perms,
      [page, &perms](const QUrl &origin, QWebEnginePage::Feature f) {
        perms.handleFeatureRequest(page, origin, f);
      });
#endif

  Downloader dl;
  dl.attach(profile);

  AppWindow win(profile, page);
  win.show();

  page->load(QUrl("https://web.whatsapp.com/"));

  return app.exec();
}
