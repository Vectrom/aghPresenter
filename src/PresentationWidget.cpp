#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
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

    m_paintImages.resize(presentation.GetNumberOfPages() + 1);
}

void PresentationWidget::NextPage()
{
    if (m_currentPage + 1 >= m_presentation.GetNumberOfPages())
        return;
        
    m_currentPage++;
    setPixmap(QPixmap::fromImage(m_presentation.GetPage(m_currentPage)));
    ResizeCurrentImage();
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

void PresentationWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isDrawingEnabled)
    {
        if(!(contentsRect().contains(event->pos())))
            return;

        m_lastPoint = event->pos();
        m_isDrawing = true;
        m_isOutsideRect = false;
    }
}

void PresentationWidget::mouseMoveEvent(QMouseEvent * event)
{
    if ((event->buttons() & Qt::LeftButton) && m_isDrawing && m_isDrawingEnabled)
    {
        if (!(contentsRect().contains(event->pos())))
        {
            m_isOutsideRect = true;
            return;
        }

        if(m_isOutsideRect) {
            m_isOutsideRect = false;
            m_lastPoint = event->pos();
            return;
        }
        DrawLineTo(event->pos());
    }
}

void PresentationWidget::mouseReleaseEvent(QMouseEvent * event)
{
    if (event->button() == Qt::LeftButton && !m_isDrawingEnabled)
    {
        NextPage();
        emit nextPageRequested();
        return;
    }

    if (event->button() == Qt::LeftButton && m_isDrawing)
    {
        if (!(contentsRect().contains(event->pos())))
            return;
        DrawLineTo(event->pos());
        m_isDrawing = false;
    }
}

void PresentationWidget::paintEvent(QPaintEvent * event)
{
    ResizablePixmapLabel::paintEvent(event);

    QPainter painter(this);
    QRect eventRect = event->rect();
    painter.drawImage(eventRect, m_paintImages[m_currentPage], eventRect);
}

void PresentationWidget::resizeEvent(QResizeEvent* event)
{
    ResizeCurrentImage();
    ResizablePixmapLabel::resizeEvent(event);
}

void PresentationWidget::DrawLineTo(const QPoint& endPoint)
{
    DrawLine(m_lastPoint, endPoint, m_penColor);
    m_lastPoint = endPoint;
}

void PresentationWidget::DrawLine(const QPoint& startPoint, const QPoint& endPoint, const QColor& color)
{
    QPainter painter(&m_paintImages[m_currentPage]);

    qreal myPenWidth = 1;
    painter.setPen(QPen(color, myPenWidth, Qt::SolidLine, Qt::RoundCap,
        Qt::RoundJoin));

    painter.drawLine(startPoint, endPoint);

    int rad = (myPenWidth / 2) + 2;

    update(QRect(startPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    emit lineDrawn(startPoint, endPoint, color);
}

void PresentationWidget::SetDrawingEnabled(bool enabled)
{
    m_isDrawingEnabled = enabled;
}

const QColor& PresentationWidget::GetPenColor() const
{
    return m_penColor;
}

void PresentationWidget::SetPenColor(const QColor& color)
{
    m_penColor = color;
}

void PresentationWidget::ResizePaintImage(QImage* image, const QSize& newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(qRgba(0, 0, 0, 0));

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void PresentationWidget::ResizeCurrentImage()
{
    if (width() > m_paintImages[m_currentPage].width() || height() > m_paintImages[m_currentPage].height())
    {
        int newWidth = qMax(width() + 128, m_paintImages[m_currentPage].width());
        int newHeight = qMax(height() + 128, m_paintImages[m_currentPage].height());
        ResizePaintImage(&m_paintImages[m_currentPage], QSize(newWidth, newHeight));
        update();
    }
}
