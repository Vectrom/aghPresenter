#pragma once

#include <QLabel>

class ResizablePixmapLabel : public QLabel
{
public:
    ResizablePixmapLabel(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    void setPixmap(QPixmap const& pixmap);

protected:
    void resizeEvent(QResizeEvent* event) override;

    QPixmap m_pixmap;

private:
    void updateMargins();
};
