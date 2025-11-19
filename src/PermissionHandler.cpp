#include "PermissionHandler.h"

static inline QString kWaHost() { return QStringLiteral("web.whatsapp.com"); }

PermissionHandler::PermissionHandler(QObject *parent) : QObject(parent) {}

bool PermissionHandler::isTrustedWhatsApp(const QUrl &origin) {
  return origin.scheme().startsWith("http") && origin.host() == kWaHost();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)

void PermissionHandler::handlePermission(QWebEnginePermission perm) {
  const QUrl origin = perm.origin();

  if (!isTrustedWhatsApp(origin)) {
    perm.deny();
    return;
  }

  using PT = QWebEnginePermission::PermissionType;

  switch (perm.permissionType()) {
  case PT::Notifications:
  case PT::MediaAudioCapture:
  case PT::MediaVideoCapture:
  case PT::MediaAudioVideoCapture:
    perm.grant();
    break;

  default:
    perm.deny();
    break;
  }
}

#else

void PermissionHandler::handleFeatureRequest(QWebEnginePage *page,
                                             const QUrl &origin,
                                             QWebEnginePage::Feature f) {
  if (!isTrustedWhatsApp(origin)) {
    page->setFeaturePermission(origin, f,
                               QWebEnginePage::PermissionDeniedByUser);
    return;
  }

  switch (f) {
  case QWebEnginePage::Notifications:
  case QWebEnginePage::MediaAudioCapture:
  case QWebEnginePage::MediaVideoCapture:
  case QWebEnginePage::MediaAudioVideoCapture:
    page->setFeaturePermission(origin, f,
                               QWebEnginePage::PermissionGrantedByUser);
    break;

  default:
    page->setFeaturePermission(origin, f,
                               QWebEnginePage::PermissionDeniedByUser);
    break;
  }
}

#endif
