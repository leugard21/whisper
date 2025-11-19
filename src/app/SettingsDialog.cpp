#include "SettingsDialog.h"
#include "../system/AutostartManager.h"
#include "../system/Settings.h"

#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QVBoxLayout>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent) {
  setWindowTitle(tr("Settings"));

  m_autostart = new QCheckBox(tr("Start on login"), this);
  m_startMinimized = new QCheckBox(tr("Start minimized to tray"), this);

  auto *buttons = new QDialogButtonBox(
      QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
  m_ok = buttons->button(QDialogButtonBox::Ok);
  m_cancel = buttons->button(QDialogButtonBox::Cancel);

  auto *layout = new QVBoxLayout(this);
  layout->addWidget(m_autostart);
  layout->addWidget(m_startMinimized);
  layout->addWidget(buttons);

  connect(buttons, &QDialogButtonBox::accepted, this,
          &SettingsDialog::onAccept);
  connect(buttons, &QDialogButtonBox::rejected, this, &SettingsDialog::reject);

  loadValues();
}

void SettingsDialog::loadValues() {
  m_autostart->setChecked(AutostartManager::isEnabled());
  m_startMinimized->setChecked(Settings::startMinimized());
}

void SettingsDialog::onAccept() {
  AutostartManager::setEnabled(m_autostart->isChecked());
  Settings::setStartMinimized(m_startMinimized->isChecked());
  accept();
}
