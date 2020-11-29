#pragma once
#include "PresentationBaseWindow.h"

class PresentationWindow : public PresentationBaseWindow
{
public:
    PresentationWindow(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
        
};