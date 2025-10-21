#include "PermissionHandler.h"
#include "WebProfileManager.h"
#include <QApplication>
#include <QScreen>
#include <QWebEnginePage>
#include <QWebEngineView>

int main(int argc, char **argv) {
  QApplication::setApplicationName("Whatsie");
  QApplication::setOrganizationName("Luna");
  QApplication::setApplicationVersion("0.0.1.0");
  QApplication app(argc, argv);

  WebProfileManager profileMgr;
  auto *page = new QWebEnginePage(profileMgr.profile());

  PermissionHandler perms;
  QObject::connect(
      page, &QWebEnginePage::featurePermissionRequested, &perms,
      [ page, &perms ](const QUrl &origin, QWebEnginePage::Feature f){
        perms.handleFeatureRequest(page, origin, f);
      });

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