#include <poppler/qt5/poppler-qt5.h>
#include <QApplication>
#include <QDesktopWidget>
#include <QDropEvent>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenuBar>
#include <QMimeData>
#include <QPushButton>
#include <QWindow>

#include "MacOsUtils.h"
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
    m_toolbar.setFloatable(false);
    m_toolbar.setMovable(false);
    m_toolbar.layout()->setSpacing(7);
    setContextMenuPolicy(Qt::NoContextMenu);
    this->setCentralWidget(&m_tabWidget);
    showMaximized();

    setAcceptDrops(true);

    setIconThemeAccordingToMacOsMode();
    loadStyleSheet();

    createAndSetActions();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* event)
{
    if(event->mimeData()->hasUrls())
        event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent* event)
{
    const QList<QUrl> urls = event->mimeData()->urls();
    if (urls.empty())
        return;

    for (const auto& url : urls)
    {
        QString filePath = url.toLocalFile();
        loadPdfFile(filePath);
    }
}

bool MainWindow::event(QEvent* event)
{
    if (event->type() == QEvent::Type::PaletteChange)
    {
        setIconThemeAccordingToMacOsMode();
        loadStyleSheet();
    }

    return QMainWindow::event(event);
}

void MainWindow::createAndSetActions()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));
    QMenu* presentationMenu = menuBar()->addMenu(tr("&Presentation"));
    QMenu* settingsMenu = menuBar()->addMenu(tr("&Settings"));

    //ONLY TO TEST by developer, TO DO: remove before official publication
    QAction* test = new QAction(QIcon::fromTheme("open"), tr("TEST"), this);
    connect(test, &QAction::triggered, this, [this]() {loadPdfFile("D:\\Studia\\Niskopoziomowe\\prezentacje\\Bootloader_3.pdf"); });
    fileMenu->addAction(test);
    m_toolbar.addAction(test);

    QAction* openDocumentAction = new QAction(QIcon::fromTheme("open"), tr("Open document"), this);
    connect(openDocumentAction, &QAction::triggered, this, &MainWindow::openDocumentActionOnClick);
    fileMenu->addAction(openDocumentAction);
    m_toolbar.addAction(openDocumentAction);

    QAction* startPresentationAction = new QAction(QIcon::fromTheme("playStart"), tr("Start presentation"), this);
    connect(startPresentationAction, &QAction::triggered, this, &MainWindow::startPresentation);
    presentationMenu->addAction(startPresentationAction);
    m_toolbar.addAction(startPresentationAction);

    QAction* startPresentationFromSlideAction = new QAction(QIcon::fromTheme("playSlide"), tr("Start presentation from current slide"), this);
    connect(startPresentationFromSlideAction, &QAction::triggered, this, &MainWindow::startPresentationFromCurrentSlide);
    presentationMenu->addAction(startPresentationFromSlideAction);
    m_toolbar.addAction(startPresentationFromSlideAction);

    QAction* openSettingsAction = new QAction(QIcon::fromTheme("settings"), tr("Open settings"), this);
    connect(openSettingsAction, &QAction::triggered, this, &MainWindow::openSettingsDialog);
    settingsMenu->addAction(openSettingsAction);
    m_toolbar.addAction(openSettingsAction);
}

std::optional<Presentation> MainWindow::getCurrentPresentation()
{
    QWidget* currentWidget = m_tabWidget.currentWidget();
    if (!currentWidget)
        return std::nullopt;

    Tab* currentTab = dynamic_cast<Tab*>(currentWidget);
    if (!currentTab)
        return std::nullopt;

    return currentTab->getPresentation();
}

void MainWindow::loadPdfFile(const QString& filePath)
{
    Presentation presentation;
    if (!presentation.readPdfFile(filePath))
        return;

    m_tabWidget.AddTab(presentation);
}

void MainWindow::loadStyleSheet()
{
    QFile file(":/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet.arg(QIcon::themeName()));
}

void MainWindow::openDocumentActionOnClick()
{
    const QString filePath = QFileDialog::getOpenFileName(this, tr("Open Document"), "", tr("PDF files (*.pdf)"));

    if (filePath.isEmpty())
        return;

    loadPdfFile(filePath);
}

void MainWindow::openSettingsDialog()
{
    SettingsDialog settingsDialog(this);
    settingsDialog.exec();
}

void MainWindow::setIconThemeAccordingToMacOsMode()
{
    if (isMacOSDarkMode())
    {
        QIcon::setThemeName("dark");
    }
    else
    {
        QIcon::setThemeName("light");
    }
}

void MainWindow::startPresentation()
{
    startPresentationFromSlide(0);
}

void MainWindow::startPresentationFromCurrentSlide()
{
    QWidget* currentTabWidget = m_tabWidget.currentWidget();
    if (!currentTabWidget)
        return;

    Tab* currentTab = dynamic_cast<Tab*>(currentTabWidget);
    if (!currentTab)
        return;

    int startPage = currentTab->getNumberOfCurrentPage();
    if (startPage == -1)
        return;

    startPresentationFromSlide(startPage);
}

void MainWindow::startPresentationFromSlide(int index)
{
    std::optional<Presentation> presentation = getCurrentPresentation();
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

    if (screens.size() > 1)
    {
        PresenterWindow* presenterWindow = new PresenterWindow(presentationWindow, presentation.value(), index, this);
        presenterWindow->show();
        presenterWindow->windowHandle()->setScreen(QApplication::screens()[0]);
        presenterWindow->showFullScreen();
        connect(presentationWindow, &PresentationWindow::windowClosed, presenterWindow, &PresenterWindow::close);
        connect(presenterWindow, &PresenterWindow::windowClosed, presentationWindow, &PresentationWindow::close);
        connect(presentationWindow, &PresentationWindow::nextPageRequested, presenterWindow, &PresenterWindow::nextPage);
        connect(presenterWindow, &PresenterWindow::nextPageRequested, presentationWindow, &PresentationWindow::nextPage);
        connect(presentationWindow, &PresentationWindow::previousPageRequested, presenterWindow, &PresenterWindow::previousPage);
        connect(presenterWindow, &PresenterWindow::previousPageRequested, presentationWindow, &PresentationWindow::previousPage);
    }
}
