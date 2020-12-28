#pragma once
#include "Presentation.h"
#include "ResizablePixmapLabel.h"

class PresentationWidget : public ResizablePixmapLabel
{
    Q_OBJECT
public:
    PresentationWidget(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
    void NextPage();
    void PreviousPage();
    void NextPagePreview();
    void PreviousPagePreview();
    void DrawLine(const QPoint& startPoint, const QPoint& endPoint, const QColor& color);
    void SetDrawingEnabled(bool enabled);
    const QColor& GetPenColor() const;
    void SetPenColor(const QColor& color);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

signals:
    void nextPageRequested();
    void previousPageRequested();
    void lineDrawn(const QPoint& startPoint, const QPoint& endPoint, const QColor& color);

private:
    Presentation m_presentation;
    QColor m_penColor;
    int m_currentPage;
    bool m_isDrawingEnabled = false;
    bool m_isDrawing = false;
    bool m_isOutsideRect = false;
    QPoint m_lastPoint;
    std::vector<QImage> m_paintImages;

    void DrawLineTo(const QPoint& endPoint);
    void ResizePaintImage(QImage* image, const QSize& newSize);
    void ResizeCurrentImage();
};