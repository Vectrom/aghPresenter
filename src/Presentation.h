#pragma once

#include <QImage>
#include <QString>
#include <vector>

namespace Poppler { class Document; }

class Presentation
{
public:
    const QString& getFilePath() const;
    int getNumberOfPages() const;
    const QImage& getPage(int index) const;
    bool readPdfFile(QString const& fileName);

private:
    void renderPageToImage(Poppler::Document* document, int i, int x, int y);

    QString m_filePath;
    std::vector<QImage> m_pages;
};
