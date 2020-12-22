#include "Timer.h"

Timer::Timer(const QTime& time) :
    m_currentTime(time),
    m_startTime(time)
{
    connect(&m_timer, &QTimer::timeout, this, &Timer::DecrementTime);
    connect(&m_timer, &QTimer::timeout, this, [this] {  emit Timeout(); });
    m_timer.start(1000);
}

QString Timer::GetCurrentTime()
{
    return m_currentTime.toString();
}

void Timer::DecrementTime()
{
    if (m_currentTime == QTime(0, 0))
        return;
    
    m_currentTime = m_currentTime.addSecs(-1);
}
