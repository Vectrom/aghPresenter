#pragma once

#include <optional>
#include <QMainWindow>
#include <QToolBar>

#include "Presentation.h"
#include "TabWidget.h"

class MainWindow final : public QMainWindow
{
public:
    MainWindow(QWidget* parent = nullptr);
    void runFromCommandLine(const QStringList& filePaths, bool autoClose);

protected:
    void dragEnterEvent(QDragEnterEvent* event) override;
    void dropEvent(QDropEvent* event) override;
    bool event(QEvent* event) override;

private:
    void createAndSetActions();
    std::optional<Presentation> getCurrentPresentation();
    bool loadPdfFile(const QString& fileName);
    void loadStyleSheet();
    void openDocumentActionOnClick();
    void openSettingsDialog();
    void setIconThemeAccordingToMacOsMode();
    void startPresentation();
    void startPresentationFromCurrentSlide();
    void startPresentationFromSlide(int index);

    TabWidget m_tabWidget;
    QToolBar m_toolbar;
};