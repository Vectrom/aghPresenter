#pragma once
#include <QImage>
#include <QString>
#include <vector>

class Presentation {
public:
    bool ReadPdfFile(QString const& fileName);
    const QString& GetFilePath() const;
    size_t GetNumberOfPages() const;
    const QImage& GetPage(int index) const;
private:
    QString m_filePath;
    std::vector<QImage> m_pages;
};