#pragma once
#include <QLabel>

class ResizablePixmapLabel : public QLabel 
{
public:
    ResizablePixmapLabel(QWidget* parent = nullptr, Qt::WindowFlags f = {0});
    void setPixmap(QPixmap const& pixmap);
protected:
    void paintEvent(QPaintEvent* event) override;

private:
    QPixmap m_pixmap;
};