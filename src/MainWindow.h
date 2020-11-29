#pragma once
#include <QMainWindow>
#include <QToolBar>
#include <optional>

#include "Presentation.h"
#include "TabWidget.h"

class MainWindow : public QMainWindow {
public:
	MainWindow(QWidget* parent = nullptr);

private:
	TabWidget m_tabWidget;
	QToolBar m_toolbar;

	std::optional<Presentation> GetCurrentPresentation();
	void OpenDocumentActionOnClick();
	void LoadPdfFile(const QString& fileName);
	void StartPresentation();
	void StartPresentationFromCurrentSlide();
	void StartPresentationFromSlide(int index);
};