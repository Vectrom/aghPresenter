#include <QCheckBox>
#include <QComboBox>
#include <QColorDialog>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QSettings>
#include <QTimeEdit>
#include <QVBoxLayout>

#include "PresenterEnums.h"
#include "SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget* parent) :
    m_showOnPresentationComboBox(new QComboBox()),
    m_timerStartTime(new QTimeEdit()),
    m_timerPositionComboBox(new QComboBox()),
    QDialog(parent, Qt::WindowCloseButtonHint)
{
    setWindowTitle(tr("Settings"));

    QGroupBox* presentationGroup = new QGroupBox(tr("Presentation configuration"));
    QPushButton* backgroundColorButton = new QPushButton(tr("Choose background color"));
    connect(backgroundColorButton, &QAbstractButton::clicked, this, &SettingsDialog::ChooseBackgroundColor);

    QVBoxLayout* presentationLayout = new QVBoxLayout();
    presentationGroup->setLayout(presentationLayout);
    presentationLayout->addWidget(backgroundColorButton);

    QGroupBox* timerGroup = new QGroupBox(tr("Timer configuration"));

    QLabel* showOnPresentationLabel = new QLabel(tr("Show on presentation by default:"));
    m_showOnPresentationComboBox->addItem(tr("Nothing"));
    m_showOnPresentationComboBox->addItem(tr("Duration clock"));
    m_showOnPresentationComboBox->addItem(tr("Timer"));
   
    QLabel* timerStartTimeLabel = new QLabel(tr("Start time:"));
    m_timerStartTime->setDisplayFormat("hh:mm:ss");

    QLabel* timerPositionLabel = new QLabel(tr("Timer or clock position:"));
    m_timerPositionComboBox->addItem(tr("Left top corner"));
    m_timerPositionComboBox->addItem(tr("Right top corner"));
    m_timerPositionComboBox->addItem(tr("Left bottom corner"));
    m_timerPositionComboBox->addItem(tr("Right bottom corner"));

    QVBoxLayout* timerLayout = new QVBoxLayout();
    timerGroup->setLayout(timerLayout);
    timerLayout->addWidget(showOnPresentationLabel);
    timerLayout->addWidget(m_showOnPresentationComboBox);
    timerLayout->addWidget(timerStartTimeLabel);
    timerLayout->addWidget(m_timerStartTime);
    timerLayout->addWidget(timerPositionLabel);
    timerLayout->addWidget(m_timerPositionComboBox);


    QPushButton* closeButton = new QPushButton(tr("Close"));
    connect(closeButton, &QAbstractButton::clicked, this, &QWidget::close);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addWidget(presentationGroup);
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
    settings.setValue("backgroundColor", m_backgroundColor);
    settings.setValue("showOnPresentationTimer", m_showOnPresentationComboBox->currentIndex());
    settings.setValue("startTime", m_timerStartTime->time().toString());
    settings.setValue("timerPosition", m_timerPositionComboBox->currentIndex());
}

void SettingsDialog::RestoreSettings()
{
    QSettings settings("AGH", "AGHPresenter");
    m_backgroundColor = settings.value("backgroundColor").value<QColor>();
    m_showOnPresentationComboBox->setCurrentIndex(settings.value("showOnPresentationTimer", static_cast<int>(ShowOnPresentationTimerType::Nothing)).toInt());
    m_timerStartTime->setTime(QTime::fromString(settings.value("startTime", "00:15:00").toString()));
    m_timerPositionComboBox->setCurrentIndex(settings.value("timerPosition", static_cast<int>(TimerPosition::RightTop)).toInt());
}

void SettingsDialog::ChooseBackgroundColor()
{
    QColor color = QColorDialog::getColor(m_backgroundColor);
    if (color.isValid())
        m_backgroundColor = color;
}
