#pragma once

#include<QListWidget.h>

#include "Presentation.h"

#define PageRole Qt::UserRole + 1

class ThumbnailsListWidget : public QListWidget 
{
public:
    ThumbnailsListWidget(Presentation const& presentation, QWidget* parent = nullptr);

private:
    void ThumbnailOnClick(QListWidgetItem * thumbnail);
    Presentation m_presentation;
};