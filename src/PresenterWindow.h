#pragma once
#include "PresentationBaseWindow.h"

class PresentationWindow;

class PresenterWindow : public PresentationBaseWindow
{
public:
    PresenterWindow(PresentationWindow * presentationWindow, Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);

private:
    PresentationWindow* m_presentationWindow;
    Timer& m_durationClock;
    Timer& m_timer;
    QHBoxLayout* m_mainLayout = nullptr;
    QVBoxLayout* m_leftLayout = nullptr;
    QVBoxLayout* m_rightLayout = nullptr;
    PresentationWidget* m_nextPageWidget = nullptr;

    QPoint ScalePresenterPointToPresentationPoint(const QPoint& point);
    void SetPenColor();
    void ConnectSignals();
    void SetUpLeftLayout();
    void SetUpRightLayout();
    void SetUpClockLayout();
    void SetUpTimerLayout();
};
