#include "AppWindow.h"
#include <QScreen>
#include <QShortcut>
#include <QWebEnginePage>
#include <QWebEngineProfile>
#include <QWebEngineView>

AppWindow::AppWindow(QWebEngineProfile *profile, QWebEnginePage *page,
                     QWidget *parent)
    : QMainWindow(parent), m_profile(profile), m_page(page) {

  setupUi();
  setupShortcuts();

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