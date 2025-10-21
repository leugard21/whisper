#include "PermissionHandler.h"

static inline QString kWaHost() { return QStringLiteral("web.whatsapp.com"); }

PermissionHandler::PermissionHandler(QObject *parent) : QObject(parent) {}

bool PermissionHandler::isTrustedWhatsApp(const QUrl &origin) {
  return origin.scheme().startsWith("http") && origin.host() == kWaHost();
}

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
void PermissionHandler::handlePermission(QWebEnginePermission permission) {
  const QUrl origin = permission.origin();
  const auto kind = permission.permissionType();

  if (!isTrustedWhatsApp(origin)) {
    permission.deny();
    return;
  }

  using PT = QWebEnginePermission::PermissionType;
  switch (kind) {
  case PT::Notifications:
  case PT::MediaAudioCapture:
  case PT::MediaVideoCapture:
  case PT::MediaAudioVideoCapture:
    permission.grant();
    break;
  default:
    permission.deny();
    break;
  }
}
#else
void PermissionHandler::handleFeatureRequest(QWebEnginePage *page,
                                             const QUrl &origin,
                                             QWebEnginePage::Feature feature) {
  if (!isTrustedWhatsApp(origin)) {
    page->setFeaturePermission(origin, feature,
                               QWebEnginePage::PermissionDeniedByUser);
    return;
  }

  switch (feature) {
  case QWebEnginePage::Notifications:
  case QWebEnginePage::MediaAudioCapture:
  case QWebEnginePage::MediaVideoCapture:
  case QWebEnginePage::MediaAudioVideoCapture:
    page->setFeaturePermission(origin, feature,
                               QWebEnginePage::PermissionGrantedByUser);
    break;
  default:
    page->setFeaturePermission(origin, feature,
                               QWebEnginePage::PermissionDeniedByUser);
    break;
  }
}
#endif
