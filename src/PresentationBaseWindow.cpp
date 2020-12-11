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
    else if (event->key() == Qt::Key_Right)
    {
        NextPage();
        emit nextPageRequested();
    }
    else if (event->key() == Qt::Key_Left)
    {
        PreviousPage();
        emit previousPageRequested();
    }
}
