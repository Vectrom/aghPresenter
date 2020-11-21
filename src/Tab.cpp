#include <QHBoxLayout>
#include <QPainter>

#include "Tab.h"

Tab::Tab(Presentation const& presentation, QWidget* parent) :
    QWidget(parent),
	m_pageImage(new QLabel()),
    m_presentation(presentation),
	m_thumbnailsList(new ThumbnailsListWidget(presentation))
{
	QHBoxLayout* tabContentLayout = new QHBoxLayout();
	setLayout(tabContentLayout);

	tabContentLayout->addWidget(m_thumbnailsList);
	connect(m_thumbnailsList, &QListWidget::itemSelectionChanged, this, &Tab::PageOnChanged);
	connect(m_thumbnailsList, &QListWidget::itemClicked, this, &Tab::PageOnChanged);

	m_pageImage->setPixmap(QPixmap::fromImage(m_presentation.GetPage(0)));
	tabContentLayout->addWidget(m_pageImage);
}

void Tab::SetPresentationPage(int index)
{
	if (index < 0 || index >= m_presentation.GetNumberOfPages())
		return;

	m_pageImage->setPixmap(QPixmap::fromImage(m_presentation.GetPage(index)));
}

void Tab::paintEvent(QPaintEvent* event)
{


}

void Tab::PageOnChanged()
{
	QListWidgetItem* currentItem = m_thumbnailsList->selectedItems()[0];
	SetPresentationPage(currentItem->data(PageRole).toInt());
}
