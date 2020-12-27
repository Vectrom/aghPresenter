#include <QScreen>
#include <QSettings>
#include <QTime>
#include <QTimer>
#include <QVBoxLayout>

#include "Presentation.h"
#include "PresentationWindow.h"
#include "PresentationWidget.h"

#include "Timer.h"

PresentationWindow::PresentationWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : PresentationBaseWindow(presentation, startPage, parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 1, 1);
    setLayout(layout);

    layout->addWidget(&m_presentationWidget);

    m_presentationWidget.setLayout(&m_presentationLayout);

    QSettings settings("AGH", "AGHPresenter");
    if (settings.value("showTimer", true).toBool())
    {
        const TimerPosition timerPosition = static_cast<TimerPosition>(settings.value("timerPosition", 1).toInt());
        const QTime time = QTime::fromString(settings.value("startTime", "00:15:00").toString());
        ConfigureTimer(timerPosition, time);
    }
}

void PresentationWindow::ConfigureTimer(const TimerPosition& timerPosition, const QTime& time)
{   
    m_presentationLayout.addWidget(&m_lcdTimer);
    m_lcdTimer.setMinimumSize(0.2 * screen()->size().width(), 0.05 * screen()->size().height());
    m_lcdTimer.setDigitCount(8);
    m_lcdTimer.setFrameStyle(QFrame::NoFrame);
    
    m_lcdTimer.display(time.toString());
    Timer* timer = new Timer(time);
    connect(timer, &Timer::Timeout, this, [this, timer]() { m_lcdTimer.display(timer->GetCurrentTime()); });

    switch (timerPosition)
    {
    case TimerPosition::LeftTop:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignLeft | Qt::AlignTop);
        break;
    case TimerPosition::RightTop:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignRight | Qt::AlignTop);
        break;
    case TimerPosition::LeftBottom:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignLeft | Qt::AlignBottom);
        break;
    case TimerPosition::RightBottom:
        m_presentationLayout.setAlignment(&m_lcdTimer, Qt::AlignRight | Qt::AlignBottom);
        break;
    }

}
