#include "Downloader.h"

#include <QDir>
#include <QFileDialog>
#include <QFileInfo>
#include <QStandardPaths>
#include <QWebEngineDownloadRequest>
#include <QWebEngineProfile>

Downloader::Downloader(QObject *parent) : QObject(parent) {}

void Downloader::attach(QWebEngineProfile *profile) {
  connect(profile, &QWebEngineProfile::downloadRequested, this,
          &Downloader::handleDownload);
}

void Downloader::handleDownload(QWebEngineDownloadRequest *req) {
  const QString suggested =
      !req->downloadFileName().isEmpty()
          ? req->downloadFileName()
          : (!req->suggestedFileName().isEmpty() ? req->suggestedFileName()
                                                 : QStringLiteral("download"));

  const QString defaultDir =
      QStandardPaths::writableLocation(QStandardPaths::DownloadLocation);

  const QString path = QFileDialog::getSaveFileName(
      nullptr, tr("Save File"), QDir(defaultDir).filePath(suggested));

  if (path.isEmpty()) {
    req->cancel();
    return;
  }

  const QFileInfo fi(path);
  req->setDownloadDirectory(fi.absolutePath());
  req->setDownloadFileName(fi.fileName());
  req->accept();

  connect(req, &QWebEngineDownloadRequest::receivedBytesChanged, req, [req] {
    const qint64 done = req->receivedBytes();
    const qint64 total = req->totalBytes();

    if (total > 0)
      qInfo() << "Downloading" << req->downloadFileName() << done << "/"
              << total;
    else
      qInfo() << "Downloading" << req->downloadFileName() << done << "/?";
  });

  connect(req, &QWebEngineDownloadRequest::stateChanged, req,
          [req](QWebEngineDownloadRequest::DownloadState st) {
            switch (st) {
            case QWebEngineDownloadRequest::DownloadCompleted:
              qInfo() << "Download completed:" << req->downloadFileName();
              break;
            case QWebEngineDownloadRequest::DownloadCancelled:
              qInfo() << "Download cancelled:" << req->downloadFileName();
              break;
            case QWebEngineDownloadRequest::DownloadInterrupted:
              qWarning() << "Download interrupted:" << req->downloadFileName()
                         << "-" << req->interruptReasonString();
              break;
            default:
              break;
            }
          });
}
