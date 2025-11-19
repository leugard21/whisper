#include "NotificationHandler.h"
#include <QSystemTrayIcon>

NotificationHandler::NotificationHandler(QObject *parent) : QObject(parent) {}

void NotificationHandler::show(const QString &title, const QString &body,
                               const QIcon &icon) {
  QSystemTrayIcon tray(icon);
  tray.show();
  tray.showMessage(title, body, icon, 4000);
}
