#pragma once
#include <QApplication>

struct RunConfig {
    bool closeAfterTest { false };
    bool openFromCmd { false };
    QStringList filePaths;
};

RunConfig parseCommandLine(QApplication& qtApp);