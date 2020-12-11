#pragma once
#include "PresentationBaseWindow.h"

class PresenterWindow : public PresentationBaseWindow
{
public:
    PresenterWindow(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
        
};