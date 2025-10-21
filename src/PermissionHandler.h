#pragma once

#include <QObject>
#include <QUrl>
#include <QWebEnginePage>

class PermissionHandler : public QObject {
  Q_OBJECT

public:
  explicit PermissionHandler(QObject *parent = nullptr);

  void handleFeatureRequest(QWebEnginePage *page, const QUrl &securityOrigin,
                            QWebEnginePage::Feature Feature);

  static bool isTrustedWhastapp(const QUrl &origin);

private:
  void grant(QWebEnginePage *page, const QUrl &origin,
             QWebEnginePage::Feature f);
  void deny(QWebEnginePage *page, const QUrl &origin,
            QWebEnginePage::Feature f);
};