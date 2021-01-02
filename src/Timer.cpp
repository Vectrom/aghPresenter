#include "Timer.h"


Timer::Timer(const QTime& startTime, const TimerType& timerType)
{
    ConfigureTimer(startTime, timerType);

    connect(&m_timer, &QTimer::timeout, this, [this] {  emit Timeout(); });
}

QString Timer::GetCurrentTime() const
{
    return m_currentTime.toString();
}

void Timer::ConfigureTimer(const QTime& startTime, const TimerType& timerType)
{
    m_currentTime = startTime;
    m_startTime = startTime;
    m_timerType = timerType;

    disconnect(&m_timer, &QTimer::timeout, this, &Timer::IncrementTime);
    disconnect(&m_timer, &QTimer::timeout, this, &Timer::DecrementTime);

    switch (timerType)
    {
    case TimerType::Timer:
        connect(&m_timer, &QTimer::timeout, this, &Timer::DecrementTime);
        break;
    case TimerType::Clock:
        connect(&m_timer, &QTimer::timeout, this, &Timer::IncrementTime);
        break;
    }
}

void Timer::StartTimer()
{
    if (!m_timer.isActive())
        m_timer.start(1000);
}

void Timer::PauseTimer()
{
    m_timer.stop();
}

void Timer::ResetTimer()
{
    bool isActive = m_timer.isActive();

    m_currentTime = m_startTime;
    emit Timeout();

    if (isActive)
    {
        PauseTimer();
        StartTimer();
    }
}

void Timer::DecrementTime()
{
    if (m_currentTime == QTime(0, 0))
        return;

    m_currentTime = m_currentTime.addSecs(-1);
}

void Timer::IncrementTime()
{
    m_currentTime = m_currentTime.addSecs(1);
}
