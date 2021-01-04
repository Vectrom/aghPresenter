#include "Timer.h"

Timer::Timer(const QTime& startTime, const TimerType& timerType)
{
    configureTimer(startTime, timerType);

    connect(&m_timer, &QTimer::timeout, this, [this] {  emit timeout(); });
}

void Timer::configureTimer(const QTime& startTime, const TimerType& timerType)
{
    m_currentTime = startTime;
    m_startTime = startTime;
    m_timerType = timerType;

    disconnect(&m_timer, &QTimer::timeout, this, &Timer::incrementTime);
    disconnect(&m_timer, &QTimer::timeout, this, &Timer::decrementTime);

    switch (timerType)
    {
    case TimerType::Timer:
        connect(&m_timer, &QTimer::timeout, this, &Timer::decrementTime);
        break;
    case TimerType::Clock:
        connect(&m_timer, &QTimer::timeout, this, &Timer::incrementTime);
        break;
    }
}

QString Timer::getCurrentTime() const
{
    return m_currentTime.toString();
}

void Timer::pauseTimer()
{
    m_timer.stop();
}

void Timer::resetTimer()
{
    bool isActive = m_timer.isActive();

    m_currentTime = m_startTime;
    emit timeout();

    if (isActive)
    {
        pauseTimer();
        startTimer();
    }
}

void Timer::startTimer()
{
    if (!m_timer.isActive())
        m_timer.start(1000);
}

void Timer::decrementTime()
{
    if (m_currentTime == QTime(0, 0))
        return;

    m_currentTime = m_currentTime.addSecs(-1);
}

void Timer::incrementTime()
{
    m_currentTime = m_currentTime.addSecs(1);
}
