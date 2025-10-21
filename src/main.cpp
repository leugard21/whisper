#include "ClientPage.h"
#include "PermissionHandler.h"
#include "WebProfileManager.h"
#include <QApplication>
#include <QScreen>
#include <QWebEngineView>

int main(int argc, char *argv[]) {
  QApplication::setApplicationName("Whatsie");
  QApplication::setOrganizationName("Whatsie");
  QApplication::setApplicationVersion("0.1.0");
  QApplication app(argc, argv);

  WebProfileManager profileMgr;
  auto *page = new ClientPage(profileMgr.profile());

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

  auto *view = new QWebEngineView;
  view->setPage(page);
  view->setWindowTitle("Whatsie");
  view->resize(1100, 760);
  view->setUrl(QUrl("https://web.whatsapp.com/"));

  const QRect avail = view->screen()->availableGeometry();
  view->move(avail.center() - view->rect().center());
  view->show();

  return app.exec();
}
