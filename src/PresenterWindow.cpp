#include <QCheckBox>
#include <QColorDialog>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QKeyEvent>
#include <QPushButton>
#include <QRect>

#include "MacOsUtils.h"
#include "Presentation.h"
#include "PresentationWidget.h"
#include "PresentationWindow.h"
#include "PresenterWindow.h"

PresenterWindow::PresenterWindow(PresentationWindow* presentationWindow, Presentation const& presentation, int startPage, QWidget* parent)
    :
    PresentationBaseWindow(presentation, startPage, parent),
    m_durationClock(presentationWindow->getDurationClock()),
    m_timer(presentationWindow->getTimer()),
    m_mainLayout(new QHBoxLayout()),
    m_leftLayout(new QVBoxLayout()),
    m_rightLayout(new QVBoxLayout()),
    m_nextPageWidget(new PresentationWidget(presentation, startPage + 1, true)),
    m_presentationWindow(presentationWindow)
{
    setLayout(m_mainLayout);

    setUpLeftLayout();
    setUpRightLayout();

    connectSignals();
    setCurrentSlideNumber();
}

void PresenterWindow::clearDrawingsOnCurrentSlide()
{
    m_presentationWidget.clearCurrentPaintImage();
    m_presentationWindow->m_presentationWidget.clearCurrentPaintImage();
}

void PresenterWindow::connectSignals()
{
    //next previous page connections
    connect(this, &PresentationBaseWindow::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPage);
    connect(this, &PresentationBaseWindow::nextPageRequested, this, &PresenterWindow::setCurrentSlideNumber);
    connect(this, &PresentationBaseWindow::previousPageRequested, m_nextPageWidget, &PresentationWidget::previousPage);
    connect(this, &PresentationBaseWindow::previousPageRequested, this, &PresenterWindow::setCurrentSlideNumber);

    connect(m_presentationWindow, &PresentationBaseWindow::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPage);
    connect(m_presentationWindow, &PresentationBaseWindow::nextPageRequested, this, &PresenterWindow::setCurrentSlideNumber);
    connect(m_presentationWindow, &PresentationBaseWindow::previousPageRequested, m_nextPageWidget, &PresentationWidget::previousPage);
    connect(m_presentationWindow, &PresentationBaseWindow::previousPageRequested, this, &PresenterWindow::setCurrentSlideNumber);

    connect(&m_presentationWidget, &PresentationWidget::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPage);
    connect(&m_presentationWidget, &PresentationWidget::nextPageRequested, &(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPage);
    connect(&m_presentationWidget, &PresentationWidget::nextPageRequested, this, &PresenterWindow::setCurrentSlideNumber);

    connect(m_nextPageWidget, &PresentationWidget::nextPageRequested, &m_presentationWidget, &PresentationWidget::nextPage);
    connect(m_nextPageWidget, &PresentationWidget::nextPageRequested, &(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPage);
    connect(m_nextPageWidget, &PresentationWidget::nextPageRequested, this, &PresenterWindow::setCurrentSlideNumber);

    connect(&(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPage);
    connect(&(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPageRequested, &m_presentationWidget, &PresentationWidget::nextPage);
    connect(&(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPageRequested, this, &PresenterWindow::setCurrentSlideNumber);

    //draw connections
    connect(&m_presentationWidget, &PresentationWidget::lineDrawn, this, [=](const QPoint& start, const QPoint& end, const QColor& color, int width) {

        QPoint scaledStart = scalePresenterPointToPresentationPoint(start);
        QPoint scaledEnd = scalePresenterPointToPresentationPoint(end);
        m_presentationWindow->m_presentationWidget.drawLine(scaledStart, scaledEnd, color, width);
    });
}

void PresenterWindow::nextPageOnAllWidgets()
{
    m_presentationWidget.nextPage();
    m_nextPageWidget->nextPage();
    m_presentationWindow->m_presentationWidget.nextPage();
}

void PresenterWindow::previousPageOnAllWidgets()
{
    m_presentationWidget.previousPage();
    m_nextPageWidget->previousPage();
    m_presentationWindow->m_presentationWidget.previousPage();
}

QPoint PresenterWindow::scalePresenterPointToPresentationPoint(const QPoint& point)
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

void PresenterWindow::setCurrentSlideNumber()
{
    int currentPageNumber = m_presentationWidget.getCurrentPageNumber();
    int numberOfPages = m_presentationWidget.getNumberOfPages();

    QString text("Slide %1 of %2");
    m_currentSlideNumber.setText(text.arg(currentPageNumber, 2).arg(numberOfPages, 2));
}

void PresenterWindow::setPenColor()
{
    QColor color = QColorDialog::getColor(m_presentationWidget.getPenColor());
    if (color.isValid())
        m_presentationWidget.setPenColor(color);
}

void PresenterWindow::setPenWidth()
{
    bool result;
    int newWidth = QInputDialog::getInt(this, tr("Pen width"), tr("Select pen width (1-50):"), m_presentationWidget.getPenWidth(), 1, 50, 1, &result);

    if (result)
        m_presentationWidget.setPenWidth(newWidth);
}

void PresenterWindow::setUpClockLayout()
{
    m_durationClock.startTimer();
    m_timer.startTimer();
    QHBoxLayout* clockLayout = new QHBoxLayout();
    m_rightLayout->addLayout(clockLayout, 1);

    QLabel* clockDescriptionLabel = new QLabel(tr("Duration: "));
    clockLayout->addWidget(clockDescriptionLabel, 1);
    QFont font = clockDescriptionLabel->font();
    font.setPointSize(15);
    clockDescriptionLabel->setFont(font);

    QLabel* clockTimeLabel = new QLabel(m_durationClock.getCurrentTime());
    clockLayout->addWidget(clockTimeLabel, 1);
    clockTimeLabel->setFont(font);
    connect(&m_durationClock, &Timer::timeout, this, [this, clockTimeLabel]() { clockTimeLabel->setText(m_durationClock.getCurrentTime()); });

    QPushButton* startClockButton = new QPushButton("");
    clockLayout->addWidget(startClockButton);
    connect(startClockButton, &QPushButton::clicked, &m_durationClock, &Timer::startTimer);
    startClockButton->setFocusPolicy(Qt::NoFocus);
    startClockButton->setObjectName("playButton");

    QPushButton* stopClockButton = new QPushButton("");
    clockLayout->addWidget(stopClockButton);
    stopClockButton->setFocusPolicy(Qt::NoFocus);
    stopClockButton->setObjectName("pauseButton");
    connect(stopClockButton, &QPushButton::clicked, &m_durationClock, &Timer::pauseTimer);

    QPushButton* resetClockButton = new QPushButton("");
    resetClockButton->setFocusPolicy(Qt::NoFocus);
    resetClockButton->setObjectName("resetButton");
    connect(resetClockButton, &QPushButton::clicked, &m_durationClock, &Timer::resetTimer);
    clockLayout->addWidget(resetClockButton);
    clockLayout->addStretch(1);
}

void PresenterWindow::setUpLeftLayout()
{
    QWidget* leftWidget = new QWidget(this);
    m_mainLayout->addWidget(leftWidget, 2);
    leftWidget->setLayout(m_leftLayout);

    m_leftLayout->addWidget(&m_presentationWidget, 1);

    QHBoxLayout* drawingLayout = new QHBoxLayout();
    m_leftLayout->addLayout(drawingLayout);

    QCheckBox* drawingCheckbox = new QCheckBox();
    drawingLayout->addWidget(drawingCheckbox);
    drawingCheckbox->setFocusPolicy(Qt::NoFocus);
    drawingCheckbox->setObjectName("drawCheckBox");
    connect(drawingCheckbox, &QCheckBox::stateChanged, &m_presentationWidget, &PresentationWidget::setDrawingEnabled);

    QPushButton* colorsButton = new QPushButton("");
    drawingLayout->addWidget(colorsButton);
    colorsButton->setFocusPolicy(Qt::NoFocus);
    colorsButton->setObjectName("colorButton");
    colorsButton->setToolTip("Set pen color");
    connect(colorsButton, &QPushButton::pressed, this, &PresenterWindow::setPenColor);

    QPushButton* penSizeButton = new QPushButton("");
    drawingLayout->addWidget(penSizeButton);
    penSizeButton->setFocusPolicy(Qt::NoFocus);
    penSizeButton->setObjectName("penSizeButton");
    penSizeButton->setToolTip("Set pen width");
    connect(penSizeButton, &QPushButton::pressed, this, &PresenterWindow::setPenWidth);

    QPushButton* eraserButton = new QPushButton("");
    drawingLayout->addWidget(eraserButton);
    eraserButton->setFocusPolicy(Qt::NoFocus);
    eraserButton->setObjectName("eraserButton");
    eraserButton->setToolTip("Clear current slide");
    connect(eraserButton, &QPushButton::pressed, this, &PresenterWindow::clearDrawingsOnCurrentSlide);

    drawingLayout->addStretch(1);

    QPushButton* leftButton = new QPushButton("");
    drawingLayout->addWidget(leftButton);
    leftButton->setFocusPolicy(Qt::NoFocus);
    leftButton->setObjectName("leftButton");
    leftButton->setToolTip("Previous slide");
    connect(leftButton, &QPushButton::pressed, this, &PresenterWindow::previousPageOnAllWidgets);
    connect(leftButton, &QPushButton::pressed, this, &PresenterWindow::setCurrentSlideNumber);

    drawingLayout->addWidget(&m_currentSlideNumber, 1);
    QFont font = m_currentSlideNumber.font();
    font.setPointSize(15);
    m_currentSlideNumber.setFont(font);
    m_currentSlideNumber.setSizePolicy(QSizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed));

    QPushButton* rightButton = new QPushButton("");
    drawingLayout->addWidget(rightButton);
    rightButton->setFocusPolicy(Qt::NoFocus);
    rightButton->setObjectName("rightButton");
    rightButton->setToolTip("Next slide");
    connect(rightButton, &QPushButton::pressed, this, &PresenterWindow::nextPageOnAllWidgets);
    connect(rightButton, &QPushButton::pressed, this, &PresenterWindow::setCurrentSlideNumber);

    m_leftLayout->addStretch();
}

void PresenterWindow::setUpRightLayout()
{
    QWidget* rightWidget = new QWidget(this);
    rightWidget->setLayout(m_rightLayout);
    m_mainLayout->addWidget(rightWidget, 1);

    QLabel* nextPageTitle = new QLabel("Next page", rightWidget);
    QFont font = nextPageTitle->font();
    font.setPointSize(15);
    nextPageTitle->setFont(font);
    m_rightLayout->addWidget(nextPageTitle);

    m_nextPageWidget->setAlignment(Qt::AlignCenter);
    m_nextPageWidget->setFrameShape(QFrame::Box);
    m_rightLayout->addWidget(m_nextPageWidget, 1);

    setUpClockLayout();
    setUpTimerLayout();

    m_rightLayout->addStretch(1);
}

void PresenterWindow::setUpTimerLayout()
{
    QHBoxLayout* timerLayout = new QHBoxLayout();
    m_rightLayout->addLayout(timerLayout, 1);

    QLabel* timerDescriptionLabel = new QLabel(tr("Timer: "));
    timerLayout->addWidget(timerDescriptionLabel, 1);
    QFont font = timerDescriptionLabel->font();
    font.setPointSize(15);
    timerDescriptionLabel->setFont(font);

    QLabel* timerTimeLabel = new QLabel(m_timer.getCurrentTime());
    timerLayout->addWidget(timerTimeLabel, 1);
    timerTimeLabel->setFont(font);
    connect(&m_timer, &Timer::timeout, this, [this, timerTimeLabel]() { timerTimeLabel->setText(m_timer.getCurrentTime()); });

    QPushButton* startTimerButton = new QPushButton("");
    timerLayout->addWidget(startTimerButton);
    startTimerButton->setFocusPolicy(Qt::NoFocus);
    startTimerButton->setObjectName("playButton");
    connect(startTimerButton, &QPushButton::clicked, &m_timer, &Timer::startTimer);

    QPushButton* stopTimerButton = new QPushButton("");
    timerLayout->addWidget(stopTimerButton);
    stopTimerButton->setFocusPolicy(Qt::NoFocus);
    stopTimerButton->setObjectName("pauseButton");
    connect(stopTimerButton, &QPushButton::clicked, &m_timer, &Timer::pauseTimer);

    QPushButton* resetTimerButton = new QPushButton("");
    timerLayout->addWidget(resetTimerButton);
    resetTimerButton->setFocusPolicy(Qt::NoFocus);
    resetTimerButton->setObjectName("resetButton");
    connect(resetTimerButton, &QPushButton::clicked, &m_timer, &Timer::resetTimer);

    timerLayout->addStretch(1);
}
