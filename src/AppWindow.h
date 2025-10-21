#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QSystemTrayIcon>
#include <qaction.h>

class QWebEngineView;
class QWebEnginePage;
class QWebEngineProfile;
class QMenu;

class AppWindow : public QMainWindow {
  Q_OBJECT

public:
  AppWindow(QWebEngineProfile *profile, QWebEnginePage *page,
            QWidget *parent = nullptr);
  ~AppWindow() override;

protected:
  void closeEvent(QCloseEvent *e) override;
  void moveEvent(QMoveEvent *e) override;
  void resizeEvent(QResizeEvent *e) override;

private slots:
  void reloadPage();
  void goBack();
  void goForward();
  void toggleDevTools();
  void onTitleChanged(const QString &title);
  void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
  void showFromTray();
  void hideToTray();

private:
  void restoreFromTray();
  void setupUi();
  void setupShortcuts();
  void setupTray();
  void createDevTools();
  void destroyDevTools();
  void updateTrayUnread(int count);
  static int extractUnreadCount(const QString &title);
  void restoreWindowStateFromSettings();
  void saveWindowStateToSettings();

  QPointer<QWebEngineProfile> m_profile;
  QPointer<QWebEnginePage> m_page;
  QPointer<QWebEngineView> m_view;
  QPointer<QWebEngineView> m_devtoolsView;
  QPointer<QSystemTrayIcon> m_tray;
  QPointer<QMenu> m_trayMenu;

  bool m_shownMinimizeHint = false;
  QAction *m_actShowHide = nullptr;
};