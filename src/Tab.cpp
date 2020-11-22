#include <QHBoxLayout>
#include <QPainter>

#include "Tab.h"

Tab::Tab(Presentation const& presentation, QWidget* parent) :
    QWidget(parent),
	m_pageImageLabel(new QLabel()),
    m_presentation(presentation),
	m_thumbnailsList(new ThumbnailsListWidget(presentation))
{
	QHBoxLayout* tabContentLayout = new QHBoxLayout();
	setLayout(tabContentLayout);

	tabContentLayout->addWidget(m_thumbnailsList, 1);
	connect(m_thumbnailsList, &QListWidget::itemSelectionChanged, this, &Tab::PageOnChanged);
	connect(m_thumbnailsList, &QListWidget::itemClicked, this, &Tab::PageOnChanged);

	m_pageImagePixmap = QPixmap::fromImage(m_presentation.GetPage(0));
	m_pageImageLabel->setPixmap(m_pageImagePixmap.scaled(m_pageImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	m_pageImageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	m_pageImageLabel->setAlignment(Qt::AlignCenter);
	tabContentLayout->addWidget(m_pageImageLabel, 5);
}

void Tab::SetPresentationPage(int index)
{
	if (index < 0 || index >= m_presentation.GetNumberOfPages())
		return;

	m_pageImagePixmap = QPixmap::fromImage(m_presentation.GetPage(index));
	m_pageImageLabel->setPixmap(m_pageImagePixmap.scaled(m_pageImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void Tab::paintEvent(QPaintEvent* event)
{
	m_pageImageLabel->setPixmap(m_pageImagePixmap.scaled(m_pageImageLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	QWidget::paintEvent(event);
}

void Tab::PageOnChanged()
{
	QListWidgetItem* currentItem = m_thumbnailsList->selectedItems()[0];
	SetPresentationPage(currentItem->data(PageRole).toInt());
}
