#include <QCheckBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QRect>

#include "Presentation.h"
#include "PresentationWindow.h"
#include "PresenterWindow.h"
#include "PresentationWidget.h"

PresenterWindow::PresenterWindow(PresentationWindow* presentationWindow, Presentation const& presentation, int startPage, QWidget* parent)
    :
    m_presentationWindow(presentationWindow),
    PresentationBaseWindow(presentation, startPage, parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    //left site
    QVBoxLayout* leftLayout = new QVBoxLayout();
    QWidget* leftWidget = new QWidget(this);
    leftWidget->setLayout(leftLayout);
    layout->addWidget(leftWidget, 2);
    
    leftLayout->addWidget(&m_presentationWidget);

    QCheckBox* drawingCheckbox = new QCheckBox("Drawing");
    drawingCheckbox->setFocusPolicy(Qt::NoFocus);
    leftLayout->addWidget(drawingCheckbox);

    //right site
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
 

    //next previous page connections
    connect(this, &PresentationBaseWindow::nextPageRequested, nextPageWidget, &PresentationWidget::NextPagePreview);
    connect(this, &PresentationBaseWindow::previousPageRequested, nextPageWidget, &PresentationWidget::PreviousPagePreview);

    connect(presentationWindow, &PresentationBaseWindow::nextPageRequested, nextPageWidget, &PresentationWidget::NextPagePreview);
    connect(presentationWindow, &PresentationBaseWindow::previousPageRequested, nextPageWidget, &PresentationWidget::PreviousPagePreview);

    connect(&m_presentationWidget, &PresentationWidget::nextPageRequested, nextPageWidget, &PresentationWidget::NextPagePreview);
    connect(&m_presentationWidget, &PresentationWidget::nextPageRequested, &(presentationWindow->m_presentationWidget), &PresentationWidget::NextPage);

    connect(nextPageWidget, &PresentationWidget::nextPageRequested, &m_presentationWidget, &PresentationWidget::NextPage);
    connect(nextPageWidget, &PresentationWidget::nextPageRequested, &(presentationWindow->m_presentationWidget), &PresentationWidget::NextPage);

    connect(&(presentationWindow->m_presentationWidget), &PresentationWidget::nextPageRequested, nextPageWidget, &PresentationWidget::NextPagePreview);
    connect(&(presentationWindow->m_presentationWidget), &PresentationWidget::nextPageRequested, &m_presentationWidget, &PresentationWidget::NextPage);

    //draw connections
    connect(drawingCheckbox, &QCheckBox::stateChanged, &m_presentationWidget, &PresentationWidget::SetDrawingEnabled);
    connect(&m_presentationWidget, &PresentationWidget::lineDrawn, this, [=](const QPoint& start, const QPoint& end) {
        
        QPoint scaledStart = ScalePresenterPointToPresentationPoint(start);
        QPoint scaledEnd = ScalePresenterPointToPresentationPoint(end);
        presentationWindow->m_presentationWidget.DrawLine(scaledStart, scaledEnd);
    });
}

QPoint PresenterWindow::ScalePresenterPointToPresentationPoint(const QPoint& point)
{
    QRect presenterContentsRect = m_presentationWidget.contentsRect();
    QRect presentationContentsRect = m_presentationWindow->m_presentationWidget.contentsRect();

    double scaleFactorHeight = presentationContentsRect.height() / static_cast<double>(presenterContentsRect.height());
    double scaleFactorWidth = presentationContentsRect.width() / static_cast<double>(presenterContentsRect.width());

    QMargins presenterMargins = m_presentationWidget.contentsMargins();
    QMargins presentationMargins = m_presentationWindow->m_presentationWidget.contentsMargins();

    int scaledX = scaleFactorWidth * (point.x() - presenterMargins.left()) + presentationMargins.left();
    int scaledY = scaleFactorHeight * (point.y() - presenterMargins.top()) + presentationMargins.top();

    return QPoint(scaledX, scaledY);
}
