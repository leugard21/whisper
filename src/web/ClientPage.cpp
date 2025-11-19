#include "ClientPage.h"

#include <QDesktopServices>
#include <QUrl>
#include <QWebEngineProfile>

static inline QString kWaHost() { return QStringLiteral("web.whatsapp.com"); }

ClientPage::ClientPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent) {}

bool ClientPage::isWhatsappDomain(const QUrl &url) const {
  return url.scheme().startsWith("http") && url.host() == kWaHost();
}

void ClientPage::openExternally(const QUrl &url) const {
  QDesktopServices::openUrl(url);
}

bool ClientPage::acceptNavigationRequest(const QUrl &url, NavigationType type,
                                         bool) {
  if (isWhatsappDomain(url))
    return true;

  if (type == NavigationTypeLinkClicked || type == NavigationTypeTyped ||
      type == NavigationTypeOther) {
    openExternally(url);
    return false;
  }

  return true;
}

QWebEnginePage *ClientPage::createWindow(WebWindowType) {
  auto *p = new ClientPage(profile());
  connect(p, &QWebEnginePage::urlChanged, this, [this, p](const QUrl &u) {
    if (!u.isEmpty()) {
      if (isWhatsappDomain(u))
        this->load(u);
      else
        openExternally(u);

      p->deleteLater();
    }
  });
  return p;
}
