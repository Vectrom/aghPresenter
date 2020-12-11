#include <QScreen>
#include <QVBoxLayout>

#include "Presentation.h"
#include "PresentationWindow.h"
#include "PresentationWidget.h"

PresentationWindow::PresentationWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : PresentationBaseWindow(presentation, startPage, parent)
{
    QHBoxLayout* layout = new QHBoxLayout();
    setLayout(layout);

    layout->addWidget(&m_presentationWidget);

    QHBoxLayout* presentationLayout = new QHBoxLayout();
    m_presentationWidget.setLayout(presentationLayout);
    presentationLayout->addWidget(&m_lcdTimer);
    m_lcdTimer.setMinimumSize(0.2 * screen()->size().width(), 0.05 * screen()->size().height());
    m_lcdTimer.setDigitCount(8);
    m_lcdTimer.display("12:00:11");
    m_lcdTimer.setSegmentStyle(QLCDNumber::Flat);
    presentationLayout->setAlignment(&m_lcdTimer, Qt::AlignRight | Qt::AlignTop);
}