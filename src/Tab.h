#pragma once

#include <QLabel>
#include <QWidget>

#include "Presentation.h"

class ResizablePixmapLabel;
class ThumbnailsListWidget;

class Tab : public QWidget 
{
public:
    Tab(Presentation const& presentation, QWidget* parent = nullptr);
    const Presentation& GetPresentation() const;
    void SetPresentationPage(int index);

private:
    void PageOnChanged();
    ResizablePixmapLabel* m_pageImageLabel;
    Presentation m_presentation;
    ThumbnailsListWidget* m_thumbnailsList;
};