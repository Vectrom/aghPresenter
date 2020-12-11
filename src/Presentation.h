#pragma once
#include <QImage>
#include <QString>
#include <vector>

namespace Poppler { class Document;  }

class Presentation {
public:
    bool ReadPdfFile(QString const& fileName);
    const QString& GetFilePath() const;
    size_t GetNumberOfPages() const;
    const QImage& GetPage(int index) const;
private:
    void RenderPageToImage(Poppler::Document* document, int i, int x, int y);
    QString m_filePath;
    std::vector<QImage> m_pages;
};