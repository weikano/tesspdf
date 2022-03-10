#ifndef IMAGEFILETESSSOURCE_H
#define IMAGEFILETESSSOURCE_H

#include "tesssource.h"
class ImageFileTessSource : public TessSource
{
public:
    ImageFileTessSource(const char* url);
    ~ImageFileTessSource(){};
    void ocr() override;
private:
    const char* mImageFilePath;
};

#endif // IMAGEFILETESSSOURCE_H
