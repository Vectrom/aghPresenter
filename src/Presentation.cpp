#include <poppler/qt5/poppler-qt5.h>

#include "Presentation.h"

bool Presentation::ReadPdfFile(QString const& fileName)
{
	m_filePath = fileName;
	Poppler::Document* document = Poppler::Document::load(m_filePath);

	if(!document || document->isLocked())
	{
		delete document;
		return false;
	}
	
	document->setRenderHint(Poppler::Document::TextAntialiasing);
	document->setRenderHint(Poppler::Document::Antialiasing);
	document->setRenderHint(Poppler::Document::TextHinting);
	document->setRenderHint(Poppler::Document::TextSlightHinting);
	document->setRenderHint(Poppler::Document::ThinLineSolid);

	int numberOfPages = document->numPages();
	m_pages.resize(numberOfPages);
	for(int i = 0; i < numberOfPages; i++)
	{
		Poppler::Page* pdfPage = document->page(i);
		if(!pdfPage) 
		{
			delete pdfPage;
			return false;
		}

		m_pages[i] = pdfPage->renderToImage();
	}
	
    return true;
}

const QString& Presentation::GetFilePath() const
{
	return m_filePath;
}

const QImage& Presentation::GetPage(int index) const
{
	if (index < 0 || index >= m_pages.size())
		return QImage();

	return m_pages[index];
}
