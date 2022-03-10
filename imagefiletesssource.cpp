#include "imagefiletesssource.h"
#include "ocr.h"
#include "leptonica/allheaders.h"

ImageFileTessSource::ImageFileTessSource(const char* url) : TessSource()
{
    mImageFilePath = url;
}

void ImageFileTessSource::ocr()
{
    if(!fopen(mImageFilePath, "r")) {
        printf("image file not exists : %s\n ",mImageFilePath);
        return;
    }
    Pix* data = pixRead(mImageFilePath);
    Ocr::get_instance().ocr(data);
    pixDestroy(&data);
}
