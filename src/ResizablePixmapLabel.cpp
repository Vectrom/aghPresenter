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
}

void ResizablePixmapLabel::paintEvent(QPaintEvent * event)
{
	QLabel::setPixmap(m_pixmap.scaled(size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	QLabel::paintEvent(event);
}
