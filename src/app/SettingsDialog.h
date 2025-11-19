#pragma once

#include <QDialog>

class QCheckBox;
class QPushButton;

class SettingsDialog : public QDialog {
  Q_OBJECT

public:
  explicit SettingsDialog(QWidget *parent = nullptr);

private slots:
  void onAccept();

private:
  QCheckBox *m_autostart = nullptr;
  QCheckBox *m_startMinimized = nullptr;
  QPushButton *m_ok = nullptr;
  QPushButton *m_cancel = nullptr;

  void loadValues();
};
