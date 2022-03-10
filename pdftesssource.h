#ifndef PDFTESSSOURCE_H
#define PDFTESSSOURCE_H

#include "tesssource.h"
#include "public/fpdf_doc.h"
#include <memory>
class PdfTessSource : public TessSource
{
public:
    PdfTessSource(const char* url, const char* pwd);
    ~PdfTessSource();
    static void initPdfEngine();
    void ocr() override;
private:
    const char* mPath;
    const char* mPwd;
    void open();
    FPDF_DOCUMENT mDocument;
};

#endif // PDFTESSSOURCE_H
