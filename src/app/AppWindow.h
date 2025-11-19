#pragma once

#include <QMainWindow>
#include <QPointer>
#include <QSystemTrayIcon>

class QAction;
class QMenu;
class QWebEnginePage;
class QWebEngineProfile;
class QWebEngineView;
class SettingsDialog;

class AppWindow : public QMainWindow {
  Q_OBJECT

public:
  AppWindow(QWebEngineProfile *profile, QWebEnginePage *page,
            QWidget *parent = nullptr);
  ~AppWindow() override;

  void setStartupHidden(bool hidden) { m_startupHidden = hidden; }

protected:
  void closeEvent(QCloseEvent *event) override;
  void moveEvent(QMoveEvent *event) override;
  void resizeEvent(QResizeEvent *event) override;

private slots:
  void reloadPage();
  void goBack();
  void goForward();
  void toggleDevTools();
  void onTitleChanged(const QString &title);
  void onTrayActivated(QSystemTrayIcon::ActivationReason reason);
  void showFromTray();
  void hideToTray();
  void openSettings();

private:
  void setupUi();
  void setupShortcuts();
  void setupTray();

  void restoreWindowStateFromSettings();
  void saveWindowStateToSettings();

  void updateShowHideLabel();
  void createDevTools();
  void destroyDevTools();
  void updateTrayUnread(int count);

  static int extractUnreadCount(const QString &title);

  QPointer<QWebEngineProfile> m_profile;
  QPointer<QWebEnginePage> m_page;
  QPointer<QWebEngineView> m_view;
  QPointer<QWebEngineView> m_devtoolsView;
  QPointer<QSystemTrayIcon> m_tray;
  QPointer<QMenu> m_trayMenu;

  QAction *m_actShowHide = nullptr;
  QAction *m_actAutostart = nullptr;
  QAction *m_actStartMinimized = nullptr;
  QAction *m_actSettings = nullptr;

  bool m_shownMinimizeHint = false;
  bool m_startupHidden = false;
};
