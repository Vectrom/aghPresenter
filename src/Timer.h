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
    void configureTimer(const QTime& startTime, const TimerType& timerType);
    QString getCurrentTime() const;
    void pauseTimer();
    void resetTimer();
    void startTimer();

signals:
    void timeout();

private:
    void decrementTime();
    void incrementTime();

    QTime m_currentTime;
    QTime m_startTime;
    QTimer m_timer;
    TimerType m_timerType;
};
