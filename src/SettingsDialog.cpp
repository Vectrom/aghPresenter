#include <QCheckBox>
#include <QComboBox>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QTimeEdit>
#include <QVBoxLayout>

#include "PresenterEnums.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget* parent) :
    m_showTimerOnPresentationOption(new QCheckBox(tr("Show timer on presentation"))),
    m_timerStartTime(new QTimeEdit()),
    m_timerPositionComboBox(new QComboBox()),
    QDialog(parent, Qt::WindowCloseButtonHint)
{
    setWindowTitle(tr("Settings"));

    QGroupBox* timerGroup = new QGroupBox(tr("Timer configuration"));
   
    QLabel* timerStartTimeLabel = new QLabel(tr("Start time:"));
    m_timerStartTime->setDisplayFormat("hh:mm:ss");
    QLabel* timerPositionLabel = new QLabel(tr("Timer position:"));
    m_timerPositionComboBox->addItem(tr("Left top corner"));
    m_timerPositionComboBox->addItem(tr("Right top corner"));
    m_timerPositionComboBox->addItem(tr("Left bottom corner"));
    m_timerPositionComboBox->addItem(tr("Right bottom corner"));

    QVBoxLayout* timerLayout = new QVBoxLayout();
    timerLayout->addWidget(m_showTimerOnPresentationOption);
    timerLayout->addWidget(timerStartTimeLabel);
    timerLayout->addWidget(m_timerStartTime);
    timerLayout->addWidget(timerPositionLabel);
    timerLayout->addWidget(m_timerPositionComboBox);

    timerGroup->setLayout(timerLayout);

    QPushButton* closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QAbstractButton::clicked, this, &QWidget::close);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(timerGroup);
    mainLayout->addStretch(1);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    RestoreSettings();
    connect(this, &QDialog::finished, this, &SettingsDialog::SaveSettings);
}

void SettingsDialog::SaveSettings()
{
    QSettings settings("AGH", "AGHPresenter");
    settings.setValue("showTimer", m_showTimerOnPresentationOption->isChecked());
    settings.setValue("startTime", m_timerStartTime->time().toString());
    settings.setValue("timerPosition", m_timerPositionComboBox->currentIndex());
}

void SettingsDialog::RestoreSettings()
{
    QSettings settings("AGH", "AGHPresenter");
    m_showTimerOnPresentationOption->setChecked(settings.value("showTimer", true).toBool());
    m_timerStartTime->setTime(QTime::fromString(settings.value("startTime", "00:15:00").toString()));
    m_timerPositionComboBox->setCurrentIndex(settings.value("timerPosition", static_cast<int>(TimerPosition::RightTop)).toInt());
}
