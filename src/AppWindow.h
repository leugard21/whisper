#pragma once

#include <QMainWindow>
#include <QPointer>
#include <qmainwindow.h>
#include <qtmetamacros.h>

class QWebEngineView;
class QWebEnginePage;
class QWebEngineProfile;

class AppWindow : public QMainWindow {
  Q_OBJECT

public:
  AppWindow(QWebEngineProfile *profile, QWebEnginePage *page,
            QWidget *parent = nullptr);
  ~AppWindow() override;

private slots:
  void reloadPage();
  void goBack();
  void goForward();
  void toggleDevTools();

private:
  void setupUi();
  void setupShortcuts();
  void createDevTools();
  void destroyDevTools();

  QPointer<QWebEngineProfile> m_profile;
  QPointer<QWebEnginePage> m_page;
  QPointer<QWebEngineView> m_view;
  QPointer<QWebEngineView> m_devtoolsView;
};