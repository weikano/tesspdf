#ifndef OCR_H
#define OCR_H

#include <memory>
#include "tesseract/baseapi.h"
#include "leptonica/allheaders.h"
class Ocr
{
public:
    static Ocr& get_instance()
    {
        static Ocr instance;
        return instance;
    }
    void ocr(Pix* data) const;
    void ocr(const unsigned char* imagedata, int width, int height,
             int bytes_per_pixel, int bytes_per_line, int ppi) const;
private:
    Ocr();
    void initOcrEngine() ;
    std::unique_ptr<tesseract::TessBaseAPI> *mEngine = nullptr;
};

#endif // OCR_H
