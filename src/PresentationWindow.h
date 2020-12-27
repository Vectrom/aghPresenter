#pragma once
#include <QLCDNumber>
#include "PresentationBaseWindow.h"
#include "PresenterEnums.h"

class PresentationWindow : public PresentationBaseWindow
{
    friend class PresenterWindow;
public:
    PresentationWindow(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
        
private:
    QLCDNumber m_lcdTimer;
    QHBoxLayout m_presentationLayout;

    void ConfigureTimer(const TimerPosition& timerPosition, const QTime& time);
};
