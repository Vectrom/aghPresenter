#pragma once
#include <QLCDNumber>
#include "PresentationBaseWindow.h"
#include "PresenterEnums.h"
#include "Timer.h"


class PresentationWindow : public PresentationBaseWindow
{
    friend class PresenterWindow;
public:
    PresentationWindow(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
    void ShowTimer();
    void ShowDurationClock();
    void HideTimerAndDurationClock();
    Timer& GetDurationClock();
    Timer& GetTimer();
private:
    Timer m_durationClock;
    Timer m_timer;
    QLCDNumber m_lcdTimer;
    QHBoxLayout m_presentationLayout;

    void ConfigureTimers();
    void ShowTimeOnLCD(const Timer&);
};
