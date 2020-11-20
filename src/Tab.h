#pragma once

#include <QWidget>

#include "Presentation.h"

class Tab : public QWidget 
{
public:
    Tab(const Presentation& presentation, QWidget* parent = nullptr);

private:
    Presentation m_presentation;
};