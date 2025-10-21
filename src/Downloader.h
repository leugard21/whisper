#pragma once

#include <QObject>
#include <QPointer>

class QWebEngineProfile;
class QWebEngineDownloadRequest;

class Downloader : public QObject {
  Q_OBJECT

public:
  explicit Downloader(QObject *parent = nullptr);
  void attach(QWebEngineProfile *profile);

private:
  void handleDownload(QWebEngineDownloadRequest *request);
};