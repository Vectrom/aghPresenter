#pragma once

#include <QLCDNumber>

#include "PresentationBaseWindow.h"
#include "PresenterEnums.h"
#include "Timer.h"

class PresentationWindow final : public PresentationBaseWindow
{
    friend class PresenterWindow;

public:
    PresentationWindow(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
    Timer& getDurationClock();
    Timer& getTimer();
    void hideTimerAndDurationClock();
    void showDurationClock();
    void showTimer();

private:
    void configureTimers(const QTime& startTime, const TimerPosition& timerPosition, const ShowOnPresentationTimerType& showOnPresentation);
    void loadSettings();
    void setBackgroundColor(const QColor& backgroundColor);
    void showTimeOnLCD(const Timer&);

    Timer m_durationClock;
    Timer m_timer;
    QLCDNumber m_lcdTimer;
    QHBoxLayout m_presentationLayout;
};
