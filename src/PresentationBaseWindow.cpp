#include <QKeyEvent>
#include <QSettings>

#include "Presentation.h"
#include "PresentationBaseWindow.h"

PresentationBaseWindow::PresentationBaseWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : m_presentationWidget(PresentationWidget(presentation, startPage, this)),
    QWidget(parent, Qt::Window)
{}

void PresentationBaseWindow::nextPage()
{
    m_presentationWidget.nextPage();
}

void PresentationBaseWindow::previousPage()
{
    m_presentationWidget.previousPage();
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
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_Down ||
             event->key() == Qt::Key_Space || event->key() == Qt::Key_Return ||
             event->key() == Qt::Key_Enter)
    {
        nextPage();
        emit nextPageRequested();
    }
    else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Up)
    {
        previousPage();
        emit previousPageRequested();
    }
}

void PresentationBaseWindow::wheelEvent(QWheelEvent* event)
{
    if (event->angleDelta().y() > 0)
    {
        nextPage();
        emit nextPageRequested();
    }
    else if (event->angleDelta().y() < 0)
    {
        previousPage();
        emit previousPageRequested();
    }
}
