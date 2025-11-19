#include "AppWindow.h"
#include "../system/AutostartManager.h"
#include "../system/Settings.h"

#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QIcon>
#include <QMenu>
#include <QMoveEvent>
#include <QResizeEvent>
#include <QScreen>
#include <QSettings>
#include <QShortcut>
#include <QSystemTrayIcon>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineView>

static inline QIcon themeOrFallback(const char *themeName,
                                    const char *fallback) {
  const QIcon themed = QIcon::fromTheme(themeName);
  return !themed.isNull() ? themed : QIcon(fallback);
}

AppWindow::AppWindow(QWebEngineProfile *profile, QWebEnginePage *page,
                     QWidget *parent)
    : QMainWindow(parent), m_profile(profile), m_page(page) {
  setupUi();
  restoreWindowStateFromSettings();
  setupShortcuts();
  setupTray();

  connect(m_page, &QWebEnginePage::titleChanged, this,
          &AppWindow::onTitleChanged);

  if (m_view && m_view->url().isEmpty())
    m_view->setUrl(QUrl("https://web.whatsapp.com/"));
}

AppWindow::~AppWindow() { destroyDevTools(); }

void AppWindow::setupUi() {
  m_view = new QWebEngineView(this);
  m_view->setPage(m_page);
  setCentralWidget(m_view);

  setWindowTitle("Whisper");
  setWindowIcon(themeOrFallback("whisper", ":/icons/whisper.png"));

  resize(1100, 760);

  const QRect avail = screen()->availableGeometry();
  move(avail.center() - rect().center());
}

void AppWindow::setupShortcuts() {
  auto *sReload = new QShortcut(QKeySequence("Ctrl+R"), this);
  connect(sReload, &QShortcut::activated, this, &AppWindow::reloadPage);

  auto *sDevTools = new QShortcut(QKeySequence("Ctrl+Shift+I"), this);
  connect(sDevTools, &QShortcut::activated, this, &AppWindow::toggleDevTools);
}

void AppWindow::setupTray() {
  if (!QSystemTrayIcon::isSystemTrayAvailable())
    return;

  m_tray = new QSystemTrayIcon(this);
  m_trayMenu = new QMenu(this);

  m_actShowHide = m_trayMenu->addAction(tr("Show"));
  connect(m_actShowHide, &QAction::triggered, this, &AppWindow::showFromTray);

  m_actAutostart = m_trayMenu->addAction(tr("Start on Login"));
  m_actAutostart->setCheckable(true);
  m_actAutostart->setChecked(AutostartManager::isEnabled());
  connect(m_actAutostart, &QAction::toggled,
          [](bool en) { AutostartManager::setEnabled(en); });

  m_actStartMinimized = m_trayMenu->addAction(tr("Start Minimized"));
  m_actStartMinimized->setCheckable(true);
  m_actStartMinimized->setChecked(Settings::startMinimized());
  connect(m_actStartMinimized, &QAction::toggled,
          [](bool en) { Settings::setStartMinimized(en); });

  m_trayMenu->addSeparator();
  QAction *actReload = m_trayMenu->addAction(tr("Reload"));
  connect(actReload, &QAction::triggered, this, &AppWindow::reloadPage);

  m_trayMenu->addSeparator();
  QAction *actQuit = m_trayMenu->addAction(tr("Quit"));
  connect(actQuit, &QAction::triggered, qApp, &QApplication::quit);

  m_tray->setIcon(themeOrFallback("whisper", ":/icons/tray.png"));
  m_tray->setToolTip("Whisper");
  m_tray->setContextMenu(m_trayMenu);
  m_tray->show();

  connect(m_tray, &QSystemTrayIcon::activated, this,
          &AppWindow::onTrayActivated);
}

void AppWindow::closeEvent(QCloseEvent *e) {
  saveWindowStateToSettings();

  if (m_tray) {
    hideToTray();
    if (!m_shownMinimizeHint) {
      m_tray->showMessage(
          tr("Whisper"),
          tr("Still running here. Click the tray icon to re-open."),
          QSystemTrayIcon::Information, 3000);
      m_shownMinimizeHint = true;
    }
    e->ignore();
    return;
  }

  QMainWindow::closeEvent(e);
}

void AppWindow::moveEvent(QMoveEvent *e) {
  QMainWindow::moveEvent(e);
  if (!isMaximized()) {
    QSettings s;
    s.setValue("ui/geometry", saveGeometry());
  }
}

void AppWindow::resizeEvent(QResizeEvent *e) {
  QMainWindow::resizeEvent(e);
  if (!isMaximized()) {
    QSettings s;
    s.setValue("ui/geometry", saveGeometry());
  }
}

void AppWindow::restoreWindowStateFromSettings() {
  QSettings s;

  const QByteArray geom = s.value("ui/geometry").toByteArray();
  const bool wasMax = s.value("ui/maximized", false).toBool();

  if (!geom.isEmpty() && !wasMax)
    restoreGeometry(geom);

  if (wasMax)
    setWindowState(windowState() | Qt::WindowMaximized);
}

void AppWindow::saveWindowStateToSettings() {
  QSettings s;

  const bool maxNow = isMaximized();
  s.setValue("ui/maximized", maxNow);

  if (!maxNow)
    s.setValue("ui/geometry", saveGeometry());
  else if (!s.contains("ui/geometry"))
    s.setValue("ui/geometry", saveGeometry());
}

void AppWindow::reloadPage() {
  if (m_page)
    m_page->triggerAction(QWebEnginePage::Reload);
}

void AppWindow::goBack() {
  if (m_page)
    m_page->triggerAction(QWebEnginePage::Back);
}

void AppWindow::goForward() {
  if (m_page)
    m_page->triggerAction(QWebEnginePage::Forward);
}

void AppWindow::createDevTools() {
  if (m_devtoolsView)
    return;

  m_devtoolsView = new QWebEngineView(this);
  auto *devPage = new QWebEnginePage(m_profile, m_devtoolsView);
  m_devtoolsView->setPage(devPage);

  m_page->setDevToolsPage(devPage);

  m_devtoolsView->setWindowTitle("Whisper DevTools");
  m_devtoolsView->resize(900, 700);
  m_devtoolsView->show();
  m_devtoolsView->raise();
  m_devtoolsView->activateWindow();
}

void AppWindow::destroyDevTools() {
  if (!m_devtoolsView)
    return;

  m_page->setDevToolsPage(nullptr);
  m_devtoolsView->deleteLater();
  m_devtoolsView = nullptr;
}

void AppWindow::toggleDevTools() {
  if (m_devtoolsView)
    destroyDevTools();
  else
    createDevTools();
}

void AppWindow::onTrayActivated(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::Trigger ||
      reason == QSystemTrayIcon::DoubleClick) {
    if (isHidden() || isMinimized())
      showFromTray();
    else
      hideToTray();
  }
}

void AppWindow::updateShowHideLabel() {
  if (!m_actShowHide)
    return;

  m_actShowHide->setText((isHidden() || isMinimized()) ? tr("Show")
                                                       : tr("Hide"));
}

void AppWindow::showFromTray() {
  restoreWindowStateFromSettings();
  raise();
  activateWindow();
  updateShowHideLabel();
}

void AppWindow::hideToTray() {
  hide();
  updateShowHideLabel();
}

int AppWindow::extractUnreadCount(const QString &title) {
  if (title.startsWith('(')) {
    const int end = title.indexOf(')');
    if (end > 1) {
      bool ok = false;
      const int n = title.mid(1, end - 1).toInt(&ok);
      if (ok)
        return n;
    }
  }
  return 0;
}

void AppWindow::updateTrayUnread(int count) {
  if (!m_tray)
    return;

  if (count > 0) {
    m_tray->setIcon(
        themeOrFallback("whisper-unread", ":/icons/tray_unread.png"));
    m_tray->setToolTip(tr("Whisper — %1 unread").arg(count));
  } else {
    m_tray->setIcon(themeOrFallback("whisper", ":/icons/tray.png"));
    m_tray->setToolTip("Whisper");
  }
}

void AppWindow::onTitleChanged(const QString &title) {
  updateTrayUnread(extractUnreadCount(title));
}
