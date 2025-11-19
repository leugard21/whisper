#include "../app/AppWindow.h"
#include "../io/Downloader.h"
#include "../system/NotificationHandler.h"
#include "../system/Settings.h"
#include "../system/SingleInstance.h"
#include "../web/ClientPage.h"
#include "../web/PermissionHandler.h"
#include "../web/WebProfileManager.h"

#include <QApplication>
#include <QMessageBox>
#include <QScreen>
#include <QWebEngineNotification>
#include <QWebEngineView>

int main(int argc, char *argv[]) {
  QApplication::setApplicationName("Whisper");
  QApplication::setOrganizationName("Luna");
  QApplication::setApplicationVersion("1.1.0");

  QApplication app(argc, argv);

  SingleInstance instance(Settings::lockFilePath());
  if (!instance.tryLock()) {
    QMessageBox::information(nullptr, "Whisper", "Whisper is already running.");
    return 0;
  }

  WebProfileManager profileMgr;
  auto *profile = profileMgr.profile();
  auto *page = new ClientPage(profile);

  static NotificationHandler notify;

  profile->setNotificationPresenter(
      [&](std::unique_ptr<QWebEngineNotification> n) {
        QIcon icon(":/icons/whisper.png");
        notify.show(n->title(), n->message(), icon);
        n->show();
      });

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

  if (Settings::startMinimized()) {
    win.setStartupHidden(true);
    win.hide();
  } else {
    win.show();
  }

  page->load(QUrl("https://web.whatsapp.com/"));

  return app.exec();
}
