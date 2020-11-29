#pragma once
#include "PresentationBaseWindow.h"

class PresenterWindow : public PresentationBaseWindow
{
public:
    PresenterWindow(Presentation const& presentation, QWidget* parent = nullptr);
        
};