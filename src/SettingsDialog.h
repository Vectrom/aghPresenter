#pragma once

#include <QDialog>
#include <QWidget>

class QCheckBox;
class QTimeEdit;
class QComboBox;

class SettingsDialog : public QDialog {
public:
    SettingsDialog(QWidget* parent = nullptr);
private:
    QColor m_backgroundColor;
    QComboBox* m_showOnPresentationComboBox = nullptr;
    QTimeEdit* m_timerStartTime = nullptr;
    QComboBox* m_timerPositionComboBox = nullptr;

    void SaveSettings();
    void RestoreSettings();
    void ChooseBackgroundColor();
};