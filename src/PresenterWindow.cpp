#include <QHBoxLayout>
#include <QRect>

#include "Presentation.h"
#include "PresentationWindow.h"
#include "PresenterWindow.h"
#include "PresentationWidget.h"

PresenterWindow::PresenterWindow(PresentationWindow* presentationWindow, Presentation const& presentation, int startPage, QWidget* parent)
    : PresentationBaseWindow(presentation, startPage, parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    layout->addWidget(&m_presentationWidget, 2);

    QVBoxLayout* rightLayout = new QVBoxLayout();
    QWidget* rightWidget = new QWidget(this);
    rightWidget->setLayout(rightLayout);
    layout->addWidget(rightWidget, 1);

    QLabel* nextPageTitle = new QLabel("Next page", rightWidget);
    QFont font = nextPageTitle->font();
    font.setPointSize(15);
    nextPageTitle->setFont(font);
    rightLayout->addWidget(nextPageTitle);


    PresentationWidget* nextPageWidget = new PresentationWidget(presentation, startPage + 1, rightWidget);
    nextPageWidget->setAlignment(Qt::AlignCenter);
    nextPageWidget->setFrameShape(QFrame::Box);
    rightLayout->addWidget(nextPageWidget, 1);
    rightLayout->addStretch(1);
 
    connect(this, &PresentationBaseWindow::nextPageRequested, nextPageWidget, &PresentationWidget::NextPagePreview);
    connect(this, &PresentationBaseWindow::previousPageRequested, nextPageWidget, &PresentationWidget::PreviousPagePreview);
    connect(presentationWindow, &PresentationBaseWindow::nextPageRequested, nextPageWidget, &PresentationWidget::NextPagePreview);
    connect(presentationWindow, &PresentationBaseWindow::previousPageRequested, nextPageWidget, &PresentationWidget::PreviousPagePreview);
  
}