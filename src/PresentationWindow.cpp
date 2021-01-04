#include <QScreen>
#include <QSettings>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>

#include "Presentation.h"
#include "PresentationWidget.h"
#include "PresentationWindow.h"
#include "Timer.h"

PresentationWindow::PresentationWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : PresentationBaseWindow(presentation, startPage, parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 1, 1);
    setLayout(layout);

    layout->addWidget(&m_presentationWidget);

    m_presentationWidget.setLayout(&m_presentationLayout);

    loadSettings();
}

Timer& PresentationWindow::getDurationClock()
{
    return m_durationClock;
}

Timer& PresentationWindow::getTimer()
{
    return m_timer;
}

void PresentationWindow::hideTimerAndDurationClock()
{
    disconnect(&m_timer, &Timer::timeout, this, nullptr);
    disconnect(&m_durationClock, &Timer::timeout, this, nullptr);
    m_lcdTimer.hide();
}

void PresentationWindow::showDurationClock()
{
    showTimeOnLCD(m_durationClock);
}

void PresentationWindow::showTimer()
{
    showTimeOnLCD(m_timer);
}

void PresentationWindow::configureTimers(const QTime& startTime, const TimerPosition& timerPosition, const ShowOnPresentationTimerType& showOnPresentation)
{
    m_presentationLayout.addWidget(&m_lcdTimer);
    m_lcdTimer.setMinimumSize(0.2 * screen()->size().width(), 0.05 * screen()->size().height());
    m_lcdTimer.setDigitCount(8);
    m_lcdTimer.setFrameStyle(QFrame::NoFrame);

    m_timer.configureTimer(startTime, Timer::TimerType::Timer);

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
        hideTimerAndDurationClock();
        break;
    case ShowOnPresentationTimerType::DurationClock:
        showDurationClock();
        m_durationClock.startTimer();
        break;
    case ShowOnPresentationTimerType::Timer:
        showTimer();
        m_timer.startTimer();
        break;
    }
}

void PresentationWindow::loadSettings()
{
    QSettings settings("AGH", "AGHPresenter");
    QColor backgroundColor = settings.value("backgroundColor").value<QColor>();
    setBackgroundColor(backgroundColor);

    const QTime startTime = QTime::fromString(settings.value("startTime", "00:15:00").toString());
    const TimerPosition timerPosition = static_cast<TimerPosition>(settings.value("timerPosition", 1).toInt());
    const ShowOnPresentationTimerType showOnPresentation = static_cast<ShowOnPresentationTimerType>(settings.value("showOnPresentationTimer", 0).toInt());

    configureTimers(startTime, timerPosition, showOnPresentation);
}

void PresentationWindow::showTimeOnLCD(const Timer& timer)
{
    disconnect(&m_timer, &Timer::timeout, this, nullptr);
    disconnect(&m_durationClock, &Timer::timeout, this, nullptr);
    connect(&timer, &Timer::timeout, this, [this, &timer]() { m_lcdTimer.display(timer.getCurrentTime()); });

    m_lcdTimer.display(timer.getCurrentTime());
    m_lcdTimer.show();
}

void PresentationWindow::setBackgroundColor(const QColor& backgroundColor)
{
    QPalette pal = palette();
    pal.setColor(QPalette::Window, backgroundColor);
    setPalette(pal);
}
