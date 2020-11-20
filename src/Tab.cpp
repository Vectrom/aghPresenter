#include <QHBoxLayout>
#include <QLabel>

#include "Tab.h"

Tab::Tab(const Presentation& presentation, QWidget* parent) :
    QWidget(parent),
    m_presentation(presentation)
{
	QHBoxLayout* tabContentLayout = new QHBoxLayout();
	setLayout(tabContentLayout);

	QLabel* pageImage = new QLabel();
	pageImage->setPixmap(QPixmap::fromImage(presentation.GetPage(0)));
	tabContentLayout->addWidget(pageImage);
}
