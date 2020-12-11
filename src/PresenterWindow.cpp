#include <QHBoxLayout>

#include "Presentation.h"
#include "PresenterWindow.h"
#include "PresentationWidget.h"

PresenterWindow::PresenterWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : PresentationBaseWindow(presentation, startPage, parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    layout->addWidget(&m_presentationWidget);
}