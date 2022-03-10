#ifndef DJVUTESSSOURCE_H
#define DJVUTESSSOURCE_H

#include "tesssource.h"
#include "libdjvu/ddjvuapi.h"
class DjVuTessSource : public TessSource
{
public:
    static void initDjVuEngine();
    DjVuTessSource(const char* path, const char* pwd);
    ~DjVuTessSource();
    void ocr() override;
private:
    void open();
    const char* mPath;
    const char* mPwd;

    int handle();
    ddjvu_context_t *mContext;
    ddjvu_document_t *mDocument;
    ddjvu_format_t *pixelformat;
    int mCount;

};

#endif // DJVUTESSSOURCE_H
