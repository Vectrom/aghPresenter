#include <QFileInfo>
#include <QTabBar>

#include "Tab.h"
#include "TabWidget.h"

TabWidget::TabWidget(QWidget* parent) : QTabWidget(parent)
{
    setTabBarAutoHide(false);
    setTabsClosable(true);
    setMovable(true);
    connect(this->tabBar(), &QTabBar::tabCloseRequested, this, &QTabWidget::removeTab);
}

void TabWidget::AddTab(Presentation const& presentation)
{
    QWidget* tabContentWidget = new Tab(presentation, this);

    const QString tabName = QFileInfo(presentation.getFilePath()).baseName();
    int index = addTab(tabContentWidget, tabName);
    setCurrentIndex(index);
}
