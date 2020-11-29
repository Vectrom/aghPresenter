#include <QVBoxLayout>

#include "Presentation.h"
#include "PresentationWindow.h"
#include "ResizablePixmapLabel.h"

PresentationWindow::PresentationWindow(Presentation const& presentation, int startPage, QWidget* parent)
    : PresentationBaseWindow(parent)
{
    QVBoxLayout* layout = new QVBoxLayout();
    setLayout(layout);

    ResizablePixmapLabel* imageLabel = new ResizablePixmapLabel();
    imageLabel->setPixmap(QPixmap::fromImage(presentation.GetPage(startPage)));
    imageLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(imageLabel);
}