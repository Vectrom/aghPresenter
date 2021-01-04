#include <QCheckBox>
#include <QColorDialog>
#include <QHBoxLayout>
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
    m_presentationWindow(presentationWindow),
    m_durationClock(presentationWindow->getDurationClock()),
    m_timer(presentationWindow->getTimer()),
    m_mainLayout(new QHBoxLayout()),
    m_leftLayout(new QVBoxLayout()),
    m_rightLayout(new QVBoxLayout()),
    m_nextPageWidget(new PresentationWidget(presentation, startPage + 1)),
    PresentationBaseWindow(presentation, startPage, parent)
{
    setLayout(m_mainLayout);

    SetUpLeftLayout();
    SetUpRightLayout();

    ConnectSignals();
}

void PresenterWindow::ConnectSignals()
{
    //next previous page connections
    connect(this, &PresentationBaseWindow::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPagePreview);
    connect(this, &PresentationBaseWindow::previousPageRequested, m_nextPageWidget, &PresentationWidget::previousPagePreview);

    connect(m_presentationWindow, &PresentationBaseWindow::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPagePreview);
    connect(m_presentationWindow, &PresentationBaseWindow::previousPageRequested, m_nextPageWidget, &PresentationWidget::previousPagePreview);

    connect(&m_presentationWidget, &PresentationWidget::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPagePreview);
    connect(&m_presentationWidget, &PresentationWidget::nextPageRequested, &(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPage);

    connect(m_nextPageWidget, &PresentationWidget::nextPageRequested, &m_presentationWidget, &PresentationWidget::nextPage);
    connect(m_nextPageWidget, &PresentationWidget::nextPageRequested, &(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPage);

    connect(&(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPageRequested, m_nextPageWidget, &PresentationWidget::nextPagePreview);
    connect(&(m_presentationWindow->m_presentationWidget), &PresentationWidget::nextPageRequested, &m_presentationWidget, &PresentationWidget::nextPage);

    //draw connections
    connect(&m_presentationWidget, &PresentationWidget::lineDrawn, this, [=](const QPoint& start, const QPoint& end, const QColor& color) {

        QPoint scaledStart = ScalePresenterPointToPresentationPoint(start);
        QPoint scaledEnd = ScalePresenterPointToPresentationPoint(end);
        m_presentationWindow->m_presentationWidget.drawLine(scaledStart, scaledEnd, color);
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

void PresenterWindow::SetPenColor()
{
    QColor color = QColorDialog::getColor(m_presentationWidget.getPenColor());
    if (color.isValid())
        m_presentationWidget.setPenColor(color);
}

void PresenterWindow::SetUpClockLayout()
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

void PresenterWindow::SetUpLeftLayout()
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
    connect(colorsButton, &QPushButton::pressed, this, &PresenterWindow::SetPenColor);

    drawingLayout->addStretch(1);
    m_leftLayout->addStretch();
}

void PresenterWindow::SetUpRightLayout()
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

    SetUpClockLayout();
    SetUpTimerLayout();

    m_rightLayout->addStretch(1);
}

void PresenterWindow::SetUpTimerLayout()
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
