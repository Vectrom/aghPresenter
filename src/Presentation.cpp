#include <poppler/qt5/poppler-qt5.h>
#include <thread>
#include <QApplication>
#include <QDesktopWidget>

#include "Presentation.h"

const QString& Presentation::getFilePath() const
{
    return m_filePath;
}

int Presentation::getNumberOfPages() const
{
    return static_cast<int>(m_pages.size());
}

const QImage& Presentation::getPage(int index) const
{
    return m_pages[index];
}

bool Presentation::readPdfFile(QString const& fileName)
{
    m_filePath = fileName;
    Poppler::Document* document = Poppler::Document::load(m_filePath);

    if (!document || document->isLocked())
    {
        delete document;
        return false;
    }

    document->setRenderHint(Poppler::Document::TextAntialiasing);
    document->setRenderHint(Poppler::Document::Antialiasing);
    document->setRenderHint(Poppler::Document::TextHinting);
    document->setRenderHint(Poppler::Document::TextSlightHinting);
    document->setRenderHint(Poppler::Document::ThinLineSolid);

    int x = QApplication::desktop()->logicalDpiX();
    int y = QApplication::desktop()->logicalDpiY();

    int numberOfPages = document->numPages();
    m_pages.resize(numberOfPages);

    std::vector<std::thread> threads;
    threads.resize(numberOfPages);

    for (int i = 0; i < numberOfPages; i++)
        threads[i] = std::thread(&Presentation::renderPageToImage, this, document, i, x, y);

    for (std::thread& thread : threads)
        thread.join();

    delete document;
    return true;
}

void Presentation::renderPageToImage(Poppler::Document* document, int i, int x, int y)
{
    Poppler::Page* pdfPage = document->page(i);
    if (!pdfPage)
    {
        delete pdfPage;
        return;
    }

    m_pages[i] = pdfPage->renderToImage(x, y);
    delete pdfPage;
}
