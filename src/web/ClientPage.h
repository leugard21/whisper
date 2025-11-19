#pragma once

#include <QWebEnginePage>

class ClientPage : public QWebEnginePage {
  Q_OBJECT

public:
  explicit ClientPage(QWebEngineProfile *profile, QObject *parent = nullptr);

protected:
  bool acceptNavigationRequest(const QUrl &url, NavigationType type,
                               bool isMainFrame) override;

  QWebEnginePage *createWindow(WebWindowType type) override;

private:
  bool isWhatsappDomain(const QUrl &url) const;
  void openExternally(const QUrl &url) const;
};
