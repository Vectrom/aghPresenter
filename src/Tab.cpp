#include <QHBoxLayout>
#include <QPainter>

#include "ResizablePixmapLabel.h"
#include "Tab.h"
#include "ThumbnailsListWidget.h"

Tab::Tab(Presentation const& presentation, QWidget* parent) :
    QWidget(parent),
    m_pageImageLabel(new ResizablePixmapLabel()),
    m_presentation(presentation),
    m_thumbnailsList(new ThumbnailsListWidget(presentation))
{
    QHBoxLayout* tabContentLayout = new QHBoxLayout();
    setLayout(tabContentLayout);

    tabContentLayout->addWidget(m_thumbnailsList, 1);
    connect(m_thumbnailsList, &QListWidget::itemSelectionChanged, this, &Tab::pageOnChanged);
    connect(m_thumbnailsList, &QListWidget::itemClicked, this, &Tab::pageOnChanged);

    m_pageImageLabel->setPixmap(QPixmap::fromImage(m_presentation.getPage(0)));
    m_pageImageLabel->setAlignment(Qt::AlignCenter);
    tabContentLayout->addWidget(m_pageImageLabel, 5);
}

int Tab::getNumberOfCurrentPage() const
{
    QListWidgetItem* currentItem = m_thumbnailsList->currentItem();
    if (!currentItem)
        return -1;

    return currentItem->data(PageRole).toInt();
}

const Presentation& Tab::getPresentation() const
{
    return m_presentation;
}

void Tab::setPresentationPage(int index)
{
    if (index < 0 || index >= m_presentation.getNumberOfPages())
        return;

    m_pageImageLabel->setPixmap(QPixmap::fromImage(m_presentation.getPage(index)));
}

void Tab::pageOnChanged()
{
    QListWidgetItem* currentItem = m_thumbnailsList->selectedItems()[0];
    setPresentationPage(currentItem->data(PageRole).toInt());
}
