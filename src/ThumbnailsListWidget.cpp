#include "ThumbnailsListWidget.h"

ThumbnailsListWidget::ThumbnailsListWidget(Presentation const& presentation, QWidget* parent) :
    QListWidget(parent),
    m_presentation(presentation)
{
    setViewMode(QListView::ListMode);
    setMovement(QListView::Static);
    setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);

    const int numberOfPages = presentation.getNumberOfPages();
    for (int i = 0; i < numberOfPages; i++)
    {
        QListWidgetItem* item = new QListWidgetItem();
        item->setText(QString::number(i + 1));
        QPixmap thumbnail = QPixmap::fromImage(presentation.getPage(i));
        thumbnail = thumbnail.scaledToWidth(200, Qt::SmoothTransformation);
        item->setData(Qt::DecorationRole, thumbnail);
        item->setData(PageRole, i);
        addItem(item);
    }

    setFixedWidth(250);

    connect(this, &QListWidget::itemActivated, this, &ThumbnailsListWidget::thumbnailOnClick);
    setCurrentItem(item(0));
}

void ThumbnailsListWidget::thumbnailOnClick(QListWidgetItem*)
{

}
