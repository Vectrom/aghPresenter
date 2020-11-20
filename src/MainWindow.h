#pragma once
#include <QMainWindow>
#include <QToolBar>

#include "TabWidget.h"

class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget* parent = nullptr);

private:
	TabWidget m_tabWidget;
	QToolBar m_toolbar;

	void OpenDocumentActionOnClick();
	void LoadPdfFile(const QString& fileName);
};