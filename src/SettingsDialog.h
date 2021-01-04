#pragma once

#include <QDialog>

class QCheckBox;
class QTimeEdit;
class QComboBox;

class SettingsDialog : public QDialog
{
public:
    SettingsDialog(QWidget* parent = nullptr);

private:
    void chooseBackgroundColor();
    void restoreSettings();
    void saveSettings();

    QColor m_backgroundColor;
    QComboBox* m_showOnPresentationComboBox = nullptr;
    QComboBox* m_timerPositionComboBox = nullptr;
    QTimeEdit* m_timerStartTime = nullptr;
};
