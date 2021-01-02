#pragma once
#include <QTime>
#include <QTimer>

class Timer : public QObject
{
    Q_OBJECT

public:
    enum class TimerType
    {
        Clock,
        Timer
    };

    Timer(const QTime& startTime = QTime(0, 0, 0), const TimerType& timerType = TimerType::Clock);
    QString GetCurrentTime() const;
    void ConfigureTimer(const QTime& startTime, const TimerType& timerType);
    void StartTimer();
    void PauseTimer();
    void ResetTimer();

signals:
    void Timeout();

private:
    void IncrementTime();
    void DecrementTime();

    QTimer m_timer;
    QTime m_currentTime;
    QTime m_startTime;
    TimerType m_timerType;
};
