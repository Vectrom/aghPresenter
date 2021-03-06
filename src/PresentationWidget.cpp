#include <QHBoxLayout>
#include <QPainter>
#include <QMouseEvent>
#include "PresentationWidget.h"

PresentationWidget::PresentationWidget(Presentation const& presentation, int startPage, bool isPreview, QWidget* parent) :
    ResizablePixmapLabel(parent),
    m_currentPage(startPage),
    m_isPreview(isPreview),
    m_presentation(presentation)
    
{
    if (startPage >= presentation.getNumberOfPages())
        setPixmap(QPixmap());
    else
        setPixmap(QPixmap::fromImage(presentation.getPage(startPage)));

    setAlignment(Qt::AlignCenter);

    m_paintImages.resize(presentation.getNumberOfPages() + 1);
}

void PresentationWidget::drawLine(const QPoint& startPoint, const QPoint& endPoint, const QColor& color, int width)
{
    QPainter painter(&m_paintImages[m_currentPage]);

    painter.setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap,
        Qt::RoundJoin));

    painter.drawLine(startPoint, endPoint);

    int rad = (width / 2) + 2;

    update(QRect(startPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));

    emit lineDrawn(startPoint, endPoint, color, width);
}

void PresentationWidget::clearCurrentPaintImage()
{
    m_paintImages[m_currentPage].fill(qRgba(0, 0, 0, 0));
    update();
}

int PresentationWidget::getCurrentPageNumber() const
{
    return m_currentPage + 1;
}

int PresentationWidget::getNumberOfPages() const
{
    return m_presentation.getNumberOfPages();
}

const QColor& PresentationWidget::getPenColor() const
{
    return m_penColor;
}

int PresentationWidget::getPenWidth() const
{
    return m_penWidth;
}

void PresentationWidget::nextPage()
{
    if (m_isPreview)
    {
        nextPagePreview();
        return;
    }

    if (m_currentPage + 1 >= m_presentation.getNumberOfPages())
        return;

    m_currentPage++;
    setPixmap(QPixmap::fromImage(m_presentation.getPage(m_currentPage)));
    resizeCurrentImage();
}

void PresentationWidget::nextPagePreview()
{
    if (m_currentPage + 1 >= m_presentation.getNumberOfPages())
    {
        setPixmap(QPixmap());

        if (m_currentPage + 1 == m_presentation.getNumberOfPages())
            m_currentPage++;

        return;
    }

    m_currentPage++;
    setPixmap(QPixmap::fromImage(m_presentation.getPage(m_currentPage)));
}

void PresentationWidget::previousPage()
{

    if (m_isPreview)
    {
        previousPagePreview();
        return;
    }

    if (m_currentPage - 1 < 0)
        return;

    m_currentPage--;
    setPixmap(QPixmap::fromImage(m_presentation.getPage(m_currentPage)));
}

void PresentationWidget::previousPagePreview()
{
    if (m_currentPage - 1 < 1 || m_presentation.getNumberOfPages() == 1)
        return;

    m_currentPage--;
    setPixmap(QPixmap::fromImage(m_presentation.getPage(m_currentPage)));
}

void PresentationWidget::setDrawingEnabled(bool enabled)
{
    m_isDrawingEnabled = enabled;
}

void PresentationWidget::setPenColor(const QColor& color)
{
    m_penColor = color;
}

void PresentationWidget::setPenWidth(int width)
{
    m_penWidth = width;
}

void PresentationWidget::mouseMoveEvent(QMouseEvent* event)
{
    if ((event->buttons() & Qt::LeftButton) && m_isDrawing && m_isDrawingEnabled)
    {
        if (!(contentsRect().contains(event->pos())))
        {
            m_isOutsideRect = true;
            return;
        }

        if (m_isOutsideRect)
        {
            m_isOutsideRect = false;
            m_lastPoint = event->pos();
            return;
        }
        drawLineTo(event->pos());
    }
}

void PresentationWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && m_isDrawingEnabled)
    {
        if (!(contentsRect().contains(event->pos())))
            return;

        m_lastPoint = event->pos();
        m_isDrawing = true;
        m_isOutsideRect = false;
    }
}

void PresentationWidget::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton && !m_isDrawingEnabled)
    {
        nextPage();
        emit nextPageRequested();
        return;
    }

    if (event->button() == Qt::LeftButton && m_isDrawing)
    {
        if (!(contentsRect().contains(event->pos())))
            return;
        drawLineTo(event->pos());
        m_isDrawing = false;
    }
}

void PresentationWidget::paintEvent(QPaintEvent* event)
{
    ResizablePixmapLabel::paintEvent(event);

    QPainter painter(this);
    QRect eventRect = event->rect();
    painter.drawImage(eventRect, m_paintImages[m_currentPage], eventRect);
}

void PresentationWidget::resizeEvent(QResizeEvent* event)
{
    resizeCurrentImage();
    ResizablePixmapLabel::resizeEvent(event);
}

void PresentationWidget::drawLineTo(const QPoint& endPoint)
{
    drawLine(m_lastPoint, endPoint, m_penColor, m_penWidth);
    m_lastPoint = endPoint;
}

void PresentationWidget::resizeCurrentImage()
{
    if (width() > m_paintImages[m_currentPage].width() || height() > m_paintImages[m_currentPage].height())
    {
        int newWidth = qMax(width() + 128, m_paintImages[m_currentPage].width());
        int newHeight = qMax(height() + 128, m_paintImages[m_currentPage].height());
        resizePaintImage(&m_paintImages[m_currentPage], QSize(newWidth, newHeight));
        update();
    }
}

void PresentationWidget::resizePaintImage(QImage* image, const QSize& newSize)
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32);
    newImage.fill(qRgba(0, 0, 0, 0));

    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
