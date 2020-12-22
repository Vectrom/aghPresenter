#include <QApplication>
#include <QDesktopWidget>
#include <QFileDialog>
#include <QLabel>
#include <QPushButton>
#include <QWindow>
#include <QHBoxLayout>
#include <poppler/qt5/poppler-qt5.h>

#include "MainWindow.h"
#include "Presentation.h"
#include "PresentationWidget.h"
#include "PresentationWindow.h"
#include "PresenterWindow.h"
#include "SettingsDialog.h"
#include "Tab.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	this->addToolBar(&m_toolbar);
	this->setCentralWidget(&m_tabWidget);
	showMaximized();

	m_toolbar.addAction(tr("Open document"), this, &MainWindow::OpenDocumentActionOnClick);
	m_toolbar.addAction(tr("Start presentation"), this, &MainWindow::StartPresentation);
	m_toolbar.addAction(tr("Start presentation from current slide"), this, &MainWindow::StartPresentationFromCurrentSlide);
	m_toolbar.addAction(tr("Open settings"), this, &MainWindow::OpenSettingsDialog);
}

std::optional<Presentation> MainWindow::GetCurrentPresentation()
{
	QWidget* currentWidget = m_tabWidget.currentWidget();
	if (!currentWidget)
		return std::nullopt;

	Tab* currentTab = dynamic_cast<Tab*>(currentWidget);
	if (!currentTab)
		return std::nullopt;

	return currentTab->GetPresentation();
}

void MainWindow::OpenDocumentActionOnClick()
{
	const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Document"), "", tr("PDF files (*.pdf)"));

	if (filePath.isEmpty())
		return;

	LoadPdfFile(filePath);
}

void MainWindow::OpenSettingsDialog()
{
	SettingsDialog settingsDialog(this);
	settingsDialog.exec();
}

void MainWindow::LoadPdfFile(const QString& filePath)
{
	Presentation presentation;
	if (!presentation.ReadPdfFile(filePath))
		return;

	m_tabWidget.AddTab(presentation);
}

void MainWindow::StartPresentation()
{
	StartPresentationFromSlide(0);
}

void MainWindow::StartPresentationFromCurrentSlide()
{
	QWidget * currentTabWidget = m_tabWidget.currentWidget();
	if (!currentTabWidget)
		return;

	Tab* currentTab = dynamic_cast<Tab*>(currentTabWidget);
	if (!currentTab)
		return;

	int startPage = currentTab->GetNumberOfCurrentPage();
	if (startPage == -1)
		return;

	StartPresentationFromSlide(startPage);
}

void MainWindow::StartPresentationFromSlide(int index)
{
	std::optional<Presentation> presentation = GetCurrentPresentation();
	if (!presentation)
		return;

	const QList<QScreen*> screens = QApplication::screens();
	
	PresentationWindow* presentationWindow = new PresentationWindow(presentation.value(), index, this);
	presentationWindow->show();
	if (screens.size() == 1)
		presentationWindow->windowHandle()->setScreen(QApplication::screens()[0]);
	else
		presentationWindow->windowHandle()->setScreen(QApplication::screens()[1]);

	presentationWindow->showFullScreen();

	if (screens.size() > 1) {
		PresenterWindow* presenterWindow = new PresenterWindow(presentationWindow, presentation.value(), index, this);
		presenterWindow->show();
		presenterWindow->windowHandle()->setScreen(QApplication::screens()[0]);
		presenterWindow->showFullScreen();
		connect(presentationWindow, &PresentationWindow::windowClosed, presenterWindow, &PresenterWindow::close);
		connect(presenterWindow, &PresenterWindow::windowClosed, presentationWindow, &PresentationWindow::close);
		connect(presentationWindow, &PresentationWindow::nextPageRequested, presenterWindow, &PresenterWindow::NextPage);
		connect(presenterWindow, &PresenterWindow::nextPageRequested, presentationWindow, &PresentationWindow::NextPage);
		connect(presentationWindow, &PresentationWindow::previousPageRequested, presenterWindow, &PresenterWindow::PreviousPage);
		connect(presenterWindow, &PresenterWindow::previousPageRequested, presentationWindow, &PresentationWindow::PreviousPage);
	}
}
