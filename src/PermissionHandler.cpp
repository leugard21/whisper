#include "PermissionHandler.h"
#include <qobject.h>
#include <qurl.h>
#include <qwebenginepage.h>

static inline QString kWaOriginHost() {
  return QStringLiteral("web.whatsapp.com");
}

PermissionHandler::PermissionHandler(QObject *parent) : QObject(parent) {}

bool PermissionHandler::isTrustedWhastapp(const QUrl &origin) {
  return origin.scheme().startsWith("http") && origin.host() == kWaOriginHost();
}

void PermissionHandler::grant(QWebEnginePage *page, const QUrl &origin,
                              QWebEnginePage::Feature f) {
  page->setFeaturePermission(origin, f,
                             QWebEnginePage::PermissionGrantedByUser);
}

void PermissionHandler::deny(QWebEnginePage *page, const QUrl &origin,
                             QWebEnginePage::Feature f) {
  page->setFeaturePermission(origin, f, QWebEnginePage::PermissionDeniedByUser);
}

void PermissionHandler::handleFeatureRequest(QWebEnginePage *page,
                                             const QUrl &origin,
                                             QWebEnginePage::Feature feature) {
  if (!isTrustedWhastapp(origin)) {
    deny(page, origin, feature);
    return;
  }

  switch (feature) {
  case QWebEnginePage::Notifications:
  case QWebEnginePage::MediaAudioCapture:
  case QWebEnginePage::MediaVideoCapture:
  case QWebEnginePage::MediaAudioVideoCapture:
    grant(page, origin, feature);
    break;
  default:
    deny(page, origin, feature);
    break;
  }
}