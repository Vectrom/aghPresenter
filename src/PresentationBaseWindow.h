#pragma once
#include <QWidget>

class PresentationBaseWindow : public QWidget
{
Q_OBJECT
public:
    PresentationBaseWindow(QWidget* parent = nullptr);

protected:
    void closeEvent(QCloseEvent* event);
    void keyPressEvent(QKeyEvent* event);

signals:
    void windowClosed();
};