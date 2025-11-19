#pragma once

#include <QObject>
#include <QUrl>

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
#include <QWebEnginePermission>
#else
#include <QWebEnginePage>
#endif

class PermissionHandler : public QObject {
  Q_OBJECT

public:
  explicit PermissionHandler(QObject *parent = nullptr);

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
  void handlePermission(QWebEnginePermission permission);
#else
  void handleFeatureRequest(QWebEnginePage *page, const QUrl &origin,
                            QWebEnginePage::Feature feature);
#endif

  static bool isTrustedWhatsApp(const QUrl &origin);
};
