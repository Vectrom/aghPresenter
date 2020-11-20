#pragma once
#include <QTabWidget>

class Presentation;

class TabWidget : public QTabWidget
{
public:
    TabWidget(QWidget* parent = nullptr);
    void AddTab(const Presentation& presentation);
};