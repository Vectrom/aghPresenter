#pragma once
#include "Presentation.h"
#include "ResizablePixmapLabel.h"

class PresentationWidget : public ResizablePixmapLabel
{
public:
    PresentationWidget(Presentation const& presentation, int startPage = 0, QWidget* parent = nullptr);
    void NextPage();
    void PreviousPage();
    void NextPagePreview();
    void PreviousPagePreview();
private:
    Presentation m_presentation;
    int m_currentPage;
};