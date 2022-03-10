#include "ocr.h"
#include "tesseract/baseapi.h"

#include <tesseract/resultiterator.h>

void Ocr::ocr(Pix *image) const
{
    printf("==============begin ocr========\n");
    clock_t start = clock();
    mEngine->get()->SetImage(image);
    mEngine->get()->Recognize(0);
    tesseract::ResultIterator *ri = mEngine->get()->GetIterator();
    ri->SetLineSeparator("\r\n");
    ri->SetParagraphSeparator("");
    tesseract::PageIteratorLevel level = tesseract::RIL_BLOCK;
    if(ri != nullptr) {
        do {
            const char* word = ri->GetUTF8Text(level);
            int x1, y1, x2, y2;
            float row_height;
            float descenders;
            float ascenders;
            ri->BoundingBox(level, &x1, &y1, &x2, &y2);
            ri->RowAttributes(&row_height, &descenders, &ascenders);
            printf("para(%s)\n", word);
//            printf("content(%s) with box(%d,%d,%d,%d) with row_height(%f) with des(%f) with asc(%f)\n", word, x1, y1, x2, y2, row_height, descenders, ascenders);
            delete[] word;
        }while(ri->Next(level));
    }
//    mEngine->get()->End();
    clock_t end = clock();
    printf("=========time cost %f=============\n", (float)(end-start)/CLOCKS_PER_SEC);
}

void Ocr::ocr(const unsigned char *imagedata, int width, int height, int bytes_per_pixel, int bytes_per_line, int ppi) const
{
    printf("==============begin ocr========\n");
    clock_t start = clock();
    mEngine->get()->SetImage(imagedata, width, height, bytes_per_pixel, bytes_per_line);
    mEngine->get()->SetSourceResolution(ppi);
    mEngine->get()->Recognize(0);
    tesseract::ResultIterator *ri = mEngine->get()->GetIterator();
    ri->SetLineSeparator("\r\n");
    ri->SetParagraphSeparator("");
    tesseract::PageIteratorLevel level = tesseract::RIL_BLOCK;
    if(ri != nullptr) {
        do {
            const char* word = ri->GetUTF8Text(level);
            int x1, y1, x2, y2;
            float row_height;
            float descenders;
            float ascenders;
            ri->BoundingBox(level, &x1, &y1, &x2, &y2);
            ri->RowAttributes(&row_height, &descenders, &ascenders);
            printf("para(%s)\n", word);
//            printf("content(%s) with box(%d,%d,%d,%d) with row_height(%f) with des(%f) with asc(%f)\n", word, x1, y1, x2, y2, row_height, descenders, ascenders);
            delete[] word;
        }while(ri->Next(level));
    }
//    mEngine->get()->End();
    clock_t end = clock();
    printf("=========time cost %f=============\n", (float)(end-start)/CLOCKS_PER_SEC);
}

Ocr::Ocr()
{
    initOcrEngine();
}

void Ocr::initOcrEngine()
{
    const char *datapath = "/home/weikano/git/tessdata_fast";
    tesseract::TessBaseAPI *api = new tesseract::TessBaseAPI;
    if(api->Init(datapath, "chi_sim")) {
        fprintf(stderr, "Could not initialize tesseract\n");
    }
    mEngine = new std::unique_ptr<tesseract::TessBaseAPI>(api);
}
