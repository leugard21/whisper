#pragma once

#include <QIcon>
#include <QObject>
#include <QString>

class NotificationHandler : public QObject {
  Q_OBJECT

public:
  explicit NotificationHandler(QObject *parent = nullptr);
  void show(const QString &title, const QString &body, const QIcon &icon);
};
