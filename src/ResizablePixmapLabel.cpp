#include <QPainter>

#include "ResizablePixmapLabel.h"

ResizablePixmapLabel::ResizablePixmapLabel(QWidget* parent, Qt::WindowFlags f) :
    QLabel(parent, f)
{
    setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
}

void ResizablePixmapLabel::setPixmap(QPixmap const& pixmap)
{
    m_pixmap = pixmap;
    QLabel::setPixmap(m_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    updateMargins();
}

void ResizablePixmapLabel::resizeEvent(QResizeEvent* event)
{
    QLabel::setPixmap(m_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    updateMargins();
    QLabel::resizeEvent(event);
}

void ResizablePixmapLabel::updateMargins()
{
    int pixmapWidth = m_pixmap.width();
    int pixmapHeight = m_pixmap.height();

    if (pixmapWidth <= 0 || pixmapHeight <= 0)
        return;

    int w = width();
    int h = height();

    if (w <= 0 || h <= 0)
        return;

    if (w * pixmapHeight > h * pixmapWidth)
    {
        int margin = (w - (pixmapWidth * h / pixmapHeight)) / 2;
        setContentsMargins(margin, 0, margin, 0);
    }
    else
    {
        int margin = (h - (pixmapHeight * w / pixmapWidth)) / 2;
        setContentsMargins(0, margin, 0, margin);
    }
}
