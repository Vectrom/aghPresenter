#include <QKeyEvent>
#include "Presentation.h"
#include "PresentationBaseWindow.h"

PresentationBaseWindow::PresentationBaseWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : m_presentationWidget(PresentationWidget(presentation, startPage, this)),
    QWidget(parent, Qt::Window)
{

}

void PresentationBaseWindow::NextPage()
{
    m_presentationWidget.NextPage();
}

void PresentationBaseWindow::PreviousPage()
{
    m_presentationWidget.PreviousPage();
}

void PresentationBaseWindow::closeEvent(QCloseEvent* event)
{
    emit windowClosed();
    QWidget::closeEvent(event);
}

void PresentationBaseWindow::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Escape)
    {
        close();
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Down || event->key() == Qt::Key_Space || event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        NextPage();
        emit nextPageRequested();
    }
    else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Up)
    {
        PreviousPage();
        emit previousPageRequested();
    }
}

void PresentationBaseWindow::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        NextPage();
        emit nextPageRequested();
    }
    else if (event->angleDelta().y() < 0)
    {
        PreviousPage();
        emit previousPageRequested();
    }
}
