#include <QKeyEvent>
#include "PresentationBaseWindow.h"

PresentationBaseWindow::PresentationBaseWindow(QWidget* parent)
    : QWidget(parent, Qt::Window)
{

}

void PresentationBaseWindow::closeEvent(QCloseEvent* event)
{
    emit windowClosed();
    QWidget::closeEvent(event);
}

void PresentationBaseWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
        close();
}
