#pragma once

#include "Presentation.h"
#include "ResizablePixmapLabel.h"

class PresentationWidget final : public ResizablePixmapLabel
{
    Q_OBJECT

public:
    PresentationWidget(Presentation const& presentation, int startPage = 0, bool isPreview = false, QWidget* parent = nullptr);
    void drawLine(const QPoint& startPoint, const QPoint& endPoint, const QColor& color, int width);
    void clearCurrentPaintImage();
    int getCurrentPageNumber() const;
    int getNumberOfPages() const;
    const QColor& getPenColor() const;
    int getPenWidth() const;
    void nextPage();
    void previousPage();
    void setDrawingEnabled(bool enabled);
    void setPenColor(const QColor& color);
    void setPenWidth(int width);

signals:
    void lineDrawn(const QPoint& startPoint, const QPoint& endPoint, const QColor& color, int width);
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
    void nextPagePreview();
    void previousPagePreview();
    void resizeCurrentImage();
    void resizePaintImage(QImage* image, const QSize& newSize);

    int m_currentPage;
    bool m_isDrawing = false;
    bool m_isDrawingEnabled = false;
    bool m_isOutsideRect = false;
    bool m_isPreview = false;
    QPoint m_lastPoint;
    std::vector<QImage> m_paintImages;
    QColor m_penColor;
    int m_penWidth = 1;
    Presentation m_presentation;
};
