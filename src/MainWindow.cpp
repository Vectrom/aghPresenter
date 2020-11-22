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

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
{
	this->addToolBar(&m_toolbar);
	this->setCentralWidget(&m_tabWidget);
	showMaximized();

	m_toolbar.addAction(tr("Open document"), this, &MainWindow::OpenDocumentActionOnClick);

	// START TEST BLOCK, remove it later
	//QPushButton* button = new QPushButton("TestPresentation", this);
	//connect(button, &QPushButton::clicked, [this]() {
	//	PresentationWindow* presentation = new PresentationWindow();
	//	presentation->show();
	//	presentation->windowHandle()->setScreen(QApplication::screens()[1]);
	//	presentation->showFullScreen();
	//});

	//setCentralWidget(myLabel);
	// END TEST BLOCK, remove it later
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
