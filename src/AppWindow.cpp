#include "AppWindow.h"
#include <QApplication>
#include <QCloseEvent>
#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QMenu>
#include <QScreen>
#include <QShortcut>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineView>
#include <iostream>
#include <qaction.h>
#include <qcoreapplication.h>
#include <qicon.h>
#include <qmenu.h>
#include <qobject.h>
#include <qsystemtrayicon.h>
#include <qwebenginepage.h>

static QString resourceIcon(const QString &name) {
  const QString base = QCoreApplication::applicationDirPath();
  const QString rel =
      QDir(base).filePath(QStringLiteral("resources/icons/") + name);
  std::cout << rel.toStdString() << "\n";
  return QFileInfo::exists(rel) ? rel : QString();
}

AppWindow::AppWindow(QWebEngineProfile *profile, QWebEnginePage *page,
                     QWidget *parent)
    : QMainWindow(parent), m_profile(profile), m_page(page) {

  m_iconPathNormal = resourceIcon("tray.png");
  m_iconPathUnread = resourceIcon("tray_unread.png");

  std::cout << m_iconPathNormal.toStdString() << "\n";

  setupUi();
  setupShortcuts();
  setupTray();

  connect(m_page, &QWebEnginePage::titleChanged, this,
          &AppWindow::onTitleChanged);

  if (m_view && m_view->url().isEmpty()) {
    m_view->setUrl(QUrl("https://web.whatsapp.com/"));
  }
}

AppWindow::~AppWindow() { destroyDevTools(); }

void AppWindow::setupUi() {
  m_view = new QWebEngineView(this);
  m_view->setPage(m_page);

  setCentralWidget(m_view);
  setWindowTitle(QStringLiteral("Whatsie"));
  resize(1100, 760);

  const QRect avail = screen()->availableGeometry();
  move(avail.center() - rect().center());

  if (!m_iconPathNormal.isEmpty()) {
    setWindowIcon(QIcon(m_iconPathNormal));
  }
}

void AppWindow::setupShortcuts() {
  auto *sReload = new QShortcut(QKeySequence("Ctrl+R"), this);
  connect(sReload, &QShortcut::activated, this, &AppWindow::reloadPage);

  auto *sBack = new QShortcut(QKeySequence("Alt+Left"), this);
  connect(sBack, &QShortcut::activated, this, &AppWindow::goBack);

  auto *sForward = new QShortcut(QKeySequence("Alt+Right"), this);
  connect(sForward, &QShortcut::activated, this, &AppWindow::goForward);

  auto *sDevTools = new QShortcut(QKeySequence("Ctrl+Shift+I"), this);
  connect(sDevTools, &QShortcut::activated, this, &AppWindow::toggleDevTools);
}

void AppWindow::setupTray() {
  if (!QSystemTrayIcon::isSystemTrayAvailable())
    return;

  m_tray = new QSystemTrayIcon(this);
  m_trayMenu = new QMenu(this);

  auto *actShow = m_trayMenu->addAction(tr("Show/Hide"));
  connect(actShow, &QAction::triggered, this, &AppWindow::showFromTray);

  m_trayMenu->addSeparator();
  auto *actReload = m_trayMenu->addAction(tr("Reload"));
  connect(actReload, &QAction::triggered, this, &AppWindow::reloadPage);

  m_trayMenu->addSeparator();
  auto *actQuit = m_trayMenu->addAction(tr("Quit"));
  connect(actQuit, &QAction::triggered, qApp, &QApplication::quit);

  if (!m_iconPathNormal.isEmpty())
    m_tray->setIcon(QIcon(m_iconPathNormal));
  else
    m_tray->setIcon(windowIcon());

  m_tray->setToolTip(tr("Whatsie"));
  m_tray->setContextMenu(m_trayMenu);
  m_tray->show();

  connect(m_tray, &QSystemTrayIcon::activated, this,
          &AppWindow::onTrayActivated);
}

void AppWindow::closeEvent(QCloseEvent *e) {
  if (m_tray) {
    hideToTray();
    if (!m_shownMinimizeHint) {
      m_tray->showMessage(
          tr("Whatsie"),
          tr("Still running here. Click the tray icon to re-open."),
          QSystemTrayIcon::Information, 3000);
      m_shownMinimizeHint = true;
    }
    e->ignore();
    return;
  }
  QMainWindow::closeEvent(e);
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

  m_devtoolsView->setWindowTitle(QStringLiteral("Whatsie DevTools"));
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
    showFromTray();
  }
}

void AppWindow::showFromTray() {
  if (isHidden())
    showNormal();
  raise();
  activateWindow();
}

void AppWindow::hideToTray() { hide(); }

int AppWindow::extractUnreadCount(const QString &title) {
  if (title.size() >= 3 && title.startsWith('(')) {
    int end = title.indexOf(')');
    if (end > 1) {
      bool ok = false;
      int n = title.mid(1, end - 1).toInt(&ok);
      if (ok)
        return n;
    }
  }
  return 0;
}

void AppWindow::updateTrayUnread(int count) {
  if (!m_tray)
    return;

  if (count > 0 && !m_iconPathUnread.isEmpty())
    m_tray->setIcon(QIcon(m_iconPathUnread));
  else if (!m_iconPathNormal.isEmpty())
    m_tray->setIcon(QIcon(m_iconPathNormal));
  else
    m_tray->setIcon(windowIcon());

  if (count > 0)
    m_tray->setToolTip(tr("Whatsie — %1 unread").arg(count));
  else
    m_tray->setToolTip(tr("Whatsie"));
}

void AppWindow::onTitleChanged(const QString &title) {
  const int unread = extractUnreadCount(title);
  updateTrayUnread(unread);
}