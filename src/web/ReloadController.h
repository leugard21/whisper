#pragma once

#include <QObject>

class ReloadController : public QObject {
  Q_OBJECT

public:
  explicit ReloadController(QObject *parent = nullptr);

signals:
  void triggerReload();
};
