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
#include "PresentationWindow.h"
#include "PresenterWindow.h"
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
	connect(presentationWindow, &PresentationWindow::windowClosed, this, &QMainWindow::show);

	if (screens.size() > 1) {
		PresenterWindow* presenterWindow = new PresenterWindow(presentation.value(), this);
		presenterWindow->show();
		presenterWindow->windowHandle()->setScreen(QApplication::screens()[0]);
		presenterWindow->showFullScreen();
		connect(presentationWindow, &PresentationWindow::windowClosed, presenterWindow, &PresenterWindow::close);
		connect(presenterWindow, &PresenterWindow::windowClosed, presentationWindow, &PresentationWindow::close);
	}

	this->hide();
}
