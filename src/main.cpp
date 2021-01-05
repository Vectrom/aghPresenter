#include <QCommandLineParser>

#include "main.h"
#include "MainWindow.h"

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    RunConfig runConfig = parseCommandLine(app);

    MainWindow mainWindow;
    mainWindow.show();

    if (runConfig.openFromCmd)
        mainWindow.runFromCommandLine(runConfig.filePaths, runConfig.closeAfterTest);

    return app.exec();
}

RunConfig parseCommandLine(QApplication& qtApp) {
    QCommandLineParser parser;
    parser.addVersionOption();
    parser.addHelpOption();

    QCommandLineOption openAndCloseOption("t", QCoreApplication::translate("main", "Test run - open and close app."));
    parser.addOption(openAndCloseOption);

    parser.addPositionalArgument("filePath", QCoreApplication::translate("main", "Path to PDF document file."));

    parser.process(qtApp);

    const QStringList args = parser.positionalArguments();

    RunConfig runConfig;
    runConfig.openFromCmd = !args.empty() || parser.isSet(openAndCloseOption);
    runConfig.closeAfterTest = parser.isSet(openAndCloseOption);
    runConfig.filePaths = args;

    return runConfig;
}