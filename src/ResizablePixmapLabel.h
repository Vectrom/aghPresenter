#pragma once
#include <QLabel>

class ResizablePixmapLabel : public QLabel 
{
public:
    ResizablePixmapLabel(QWidget* parent = nullptr, Qt::WindowFlags f = {0});
    void setPixmap(QPixmap const& pixmap);
protected:
    void resizeEvent(QResizeEvent* event) override;
    void UpdateMargins();
    QPixmap m_pixmap;
private:
  
};