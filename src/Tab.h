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
    int getNumberOfCurrentPage() const;
    const Presentation& getPresentation() const;
    void setPresentationPage(int index);

private:
    void pageOnChanged();

    ResizablePixmapLabel* m_pageImageLabel;
    Presentation m_presentation;
    ThumbnailsListWidget* m_thumbnailsList;
};
