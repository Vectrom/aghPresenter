#include <QHBoxLayout>
#include "PresentationWidget.h"

PresentationWidget::PresentationWidget(Presentation const& presentation, int startPage, QWidget* parent) :
    m_presentation(presentation),
    m_currentPage(startPage),
    ResizablePixmapLabel(parent)
{
    if (startPage >= presentation.GetNumberOfPages())
        setPixmap(QPixmap());
    else
        setPixmap(QPixmap::fromImage(presentation.GetPage(startPage)));

    setAlignment(Qt::AlignCenter);
}

void PresentationWidget::NextPage()
{
    if (m_currentPage + 1 >= m_presentation.GetNumberOfPages())
        return;
        
    m_currentPage++;
    setPixmap(QPixmap::fromImage(m_presentation.GetPage(m_currentPage)));
}

void PresentationWidget::PreviousPage()
{
    if (m_currentPage - 1 < 0)
        return;

    m_currentPage--;
    setPixmap(QPixmap::fromImage(m_presentation.GetPage(m_currentPage)));
}

void PresentationWidget::NextPagePreview()
{       
    if (m_currentPage + 1 >= m_presentation.GetNumberOfPages())
    {
        setPixmap(QPixmap());

        if (m_currentPage + 1 == m_presentation.GetNumberOfPages())
            m_currentPage++;

        return;
    }

    m_currentPage++;
    setPixmap(QPixmap::fromImage(m_presentation.GetPage(m_currentPage)));
}

void PresentationWidget::PreviousPagePreview()
{
    if (m_currentPage - 1 < 1 || m_presentation.GetNumberOfPages() == 1)
        return;

    m_currentPage--;
    setPixmap(QPixmap::fromImage(m_presentation.GetPage(m_currentPage)));
}
