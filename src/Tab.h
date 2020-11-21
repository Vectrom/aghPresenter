#pragma once

#include <QLabel>
#include <QWidget>

#include "Presentation.h"
#include "ThumbnailsListWidget.h"

class Tab : public QWidget 
{
public:
    Tab(Presentation const& presentation, QWidget* parent = nullptr);
    void SetPresentationPage(int index);
protected:
    void paintEvent(QPaintEvent* event) override;
private:
    void PageOnChanged();
    QLabel* m_pageImage;
    Presentation m_presentation;
    ThumbnailsListWidget* m_thumbnailsList;
};