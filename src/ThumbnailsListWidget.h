#pragma once

#include<QListWidget>

#include "Presentation.h"

#define PageRole Qt::UserRole + 1

class ThumbnailsListWidget : public QListWidget
{
public:
    ThumbnailsListWidget(Presentation const& presentation, QWidget* parent = nullptr);

private:
    void thumbnailOnClick(QListWidgetItem* thumbnail);

    Presentation m_presentation;
};