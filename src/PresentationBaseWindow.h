#pragma once

#include <QWidget>

#include "PresentationWidget.h"

class Presentation;

class PresentationBaseWindow : public QWidget
{
    Q_OBJECT

public:
    PresentationBaseWindow(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
    void nextPage();
    void previousPage();

signals:
    void windowClosed();
    void nextPageRequested();
    void previousPageRequested();

protected:
    void closeEvent(QCloseEvent* event) override;
    void keyPressEvent(QKeyEvent* event) override;
    void wheelEvent(QWheelEvent* event) override;

    PresentationWidget m_presentationWidget;
};
