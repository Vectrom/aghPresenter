#pragma once

#include "Presentation.h"
#include "ResizablePixmapLabel.h"

class PresentationWidget final : public ResizablePixmapLabel
{
    Q_OBJECT

public:
    PresentationWidget(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
    void drawLine(const QPoint& startPoint, const QPoint& endPoint, const QColor& color);
    const QColor& getPenColor() const;
    void nextPage();
    void nextPagePreview();
    void previousPage();
    void previousPagePreview();
    void setDrawingEnabled(bool enabled);
    void setPenColor(const QColor& color);

signals:
    void lineDrawn(const QPoint& startPoint, const QPoint& endPoint, const QColor& color);
    void nextPageRequested();
    void previousPageRequested();

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    void drawLineTo(const QPoint& endPoint);
    void resizeCurrentImage();
    void resizePaintImage(QImage* image, const QSize& newSize);

    int m_currentPage;
    bool m_isDrawing = false;
    bool m_isDrawingEnabled = false;
    bool m_isOutsideRect = false;
    QPoint m_lastPoint;
    std::vector<QImage> m_paintImages;
    QColor m_penColor;
    Presentation m_presentation;
};
