#pragma once

#include "PresentationBaseWindow.h"

class PresentationWindow;

class PresenterWindow : public PresentationBaseWindow
{
public:
    PresenterWindow(PresentationWindow* presentationWindow, Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);

private:
    void clearDrawingsOnCurrentSlide();
    void connectSignals();
    void nextPageOnAllWidgets();
    void previousPageOnAllWidgets();
    QPoint scalePresenterPointToPresentationPoint(const QPoint& point);
    void setCurrentSlideNumber();
    void setPenColor();
    void setPenWidth();
    void setUpClockLayout();
    void setUpLeftLayout();
    void setUpRightLayout();
    void setUpTimerLayout();

    QLabel m_currentSlideNumber;
    Timer& m_durationClock;
    Timer& m_timer;
    QHBoxLayout* m_mainLayout = nullptr;
    QVBoxLayout* m_leftLayout = nullptr;
    QVBoxLayout* m_rightLayout = nullptr;
    PresentationWidget m_nextPageWidget;
    PresentationWindow* m_presentationWindow;
};
