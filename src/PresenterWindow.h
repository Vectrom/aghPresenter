#pragma once
#include "PresentationBaseWindow.h"

class PresentationWindow;

class PresenterWindow : public PresentationBaseWindow
{
public:
    PresenterWindow(PresentationWindow * presentationWindow, Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
        
};