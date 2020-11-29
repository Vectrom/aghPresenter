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
	connect(m_thumbnailsList, &QListWidget::itemSelectionChanged, this, &Tab::PageOnChanged);
	connect(m_thumbnailsList, &QListWidget::itemClicked, this, &Tab::PageOnChanged);

	m_pageImageLabel->setPixmap(QPixmap::fromImage(m_presentation.GetPage(0)));
	m_pageImageLabel->setAlignment(Qt::AlignCenter);
	tabContentLayout->addWidget(m_pageImageLabel, 5);
}

const Presentation& Tab::GetPresentation() const
{
	return m_presentation;
}

void Tab::SetPresentationPage(int index)
{
	if (index < 0 || index >= m_presentation.GetNumberOfPages())
		return;

	m_pageImageLabel->setPixmap(QPixmap::fromImage(m_presentation.GetPage(index)));
}

void Tab::PageOnChanged()
{
	QListWidgetItem* currentItem = m_thumbnailsList->selectedItems()[0];
	SetPresentationPage(currentItem->data(PageRole).toInt());
}
