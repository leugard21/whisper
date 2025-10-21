#include <QApplication>
#include <QScreen>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <qapplication.h>
#include <qpaintdevice.h>
#include <qurl.h>
#include <qwebengineview.h>

int main(int argc, char **argv) {
  QApplication::setApplicationName("Whatsie");
  QApplication::setOrganizationName("Luna");
  QApplication::setApplicationVersion("0.0.1.0");
  QApplication app(argc, argv);

  auto *view = new QWebEngineView;
  view->setWindowTitle("Whatsie");
  view->resize(1100, 760);
  view->setUrl(QUrl("https://web.whatsapp.com/"));

  const QRect avail = view->screen()->availableGeometry();
  view->move(avail.center() - view->rect().center());
  view->show();

  return app.exec();
}