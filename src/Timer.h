#pragma once
#include <QTime>
#include <QTimer>

class Timer : public QObject {
    Q_OBJECT

public:
    Timer(const QTime& time);
    QString GetCurrentTime();

signals:
    void Timeout();

private:
    void DecrementTime();

    QTimer m_timer;
    QTime m_currentTime;
    const QTime m_startTime;
};