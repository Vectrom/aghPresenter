#include <QScreen>
#include <QSettings>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>

#include "Presentation.h"
#include "PresentationWindow.h"
#include "PresentationWidget.h"

#include "Timer.h"

PresentationWindow::PresentationWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : PresentationBaseWindow(presentation, startPage, parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 1, 1);
    setLayout(layout);

    layout->addWidget(&m_presentationWidget);

    m_presentationWidget.setLayout(&m_presentationLayout);

    LoadSettings();
}

void PresentationWindow::ShowTimer()
{
    ShowTimeOnLCD(m_timer);
}

void PresentationWindow::ShowDurationClock()
{
    ShowTimeOnLCD(m_durationClock);
}

void PresentationWindow::HideTimerAndDurationClock()
{
    disconnect(&m_timer, &Timer::Timeout, this, nullptr);
    disconnect(&m_durationClock, &Timer::Timeout, this, nullptr);
    m_lcdTimer.hide();
}

Timer& PresentationWindow::GetDurationClock()
{
    return m_durationClock;
}

Timer& PresentationWindow::GetTimer()
{
    return m_timer;
}

void PresentationWindow::ConfigureTimers(const QTime& startTime, const TimerPosition& timerPosition, const ShowOnPresentationTimerType& showOnPresentation)
{   
    m_presentationLayout.addWidget(&m_lcdTimer);
    m_lcdTimer.setMinimumSize(0.2 * screen()->size().width(), 0.05 * screen()->size().height());
    m_lcdTimer.setDigitCount(8);
    m_lcdTimer.setFrameStyle(QFrame::NoFrame);

    m_timer.ConfigureTimer(startTime, Timer::TimerType::Timer);

    switch (timerPosition)
    {
    case TimerPosition::LeftTop:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignLeft | Qt::AlignTop);
        break;
    case TimerPosition::RightTop:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignRight | Qt::AlignTop);
        break;
    case TimerPosition::LeftBottom:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignLeft | Qt::AlignBottom);
        break;
    case TimerPosition::RightBottom:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignRight | Qt::AlignBottom);
        break;
    }

    switch (showOnPresentation)
    {
    case ShowOnPresentationTimerType::Nothing:
        HideTimerAndDurationClock();
        break;
    case ShowOnPresentationTimerType::DurationClock:
        ShowDurationClock();
        m_durationClock.StartTimer();
        break;
    case ShowOnPresentationTimerType::Timer:
        ShowTimer();
        m_timer.StartTimer();
        break;
    }
}

void PresentationWindow::ShowTimeOnLCD(const Timer& timer)
{
    disconnect(&m_timer, &Timer::Timeout, this, nullptr);
    disconnect(&m_durationClock, &Timer::Timeout, this, nullptr);
    connect(&timer, &Timer::Timeout, this, [this, &timer]() { m_lcdTimer.display(timer.GetCurrentTime()); });

    m_lcdTimer.display(timer.GetCurrentTime());
    m_lcdTimer.show();
}

void PresentationWindow::LoadSettings()
{
    QSettings settings("AGH", "AGHPresenter");
    QColor backgroundColor = settings.value("backgroundColor").value<QColor>();
    SetBackgroundColor(backgroundColor);

    const QTime startTime = QTime::fromString(settings.value("startTime", "00:15:00").toString());
    const TimerPosition timerPosition = static_cast<TimerPosition>(settings.value("timerPosition", 1).toInt());
    const ShowOnPresentationTimerType showOnPresentation = static_cast<ShowOnPresentationTimerType>(settings.value("showOnPresentationTimer", 0).toInt());

    ConfigureTimers(startTime, timerPosition, showOnPresentation);
}

void PresentationWindow::SetBackgroundColor(const QColor& backgroundColor)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, backgroundColor);
    setPalette(pal);
}
