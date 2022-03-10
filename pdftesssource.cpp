#include "pdftesssource.h"
#include "public/fpdfview.h"
#include "ocr.h"

#ifndef SK_R32_SHIFT
#define SK_R32_SHIFT 0
#endif
#ifndef SK_G32_SHIFT
#define SK_G32_SHIFT 8
#endif
#ifndef SK_A32_SHIFT
#define SK_A32_SHIFT 24
#endif
#ifndef SK_B32_SHIFT
#define SK_B32_SHIFT (16-SK_R32_SHIFT)
#endif

void PdfTessSource::initPdfEngine()
{
    FPDF_InitLibrary();
}

PdfTessSource::PdfTessSource(const char* url, const char* pwd) : TessSource(), mPath(url), mPwd(pwd)
{

}

PdfTessSource::~PdfTessSource()
{
    if(mDocument) {
        FPDF_CloseDocument(mDocument);
    }
}

void PdfTessSource::ocr()
{

    open();
    if(!mDocument) {
        return;
    }
    static int flag = FPDF_REVERSE_BYTE_ORDER | FPDF_ANNOT;
    static int format = FPDFBitmap_BGRA;
    static float max_size = 1024;

    int page_count = FPDF_GetPageCount(mDocument);
    for(int i=0;i<page_count;i++) {
        printf("=======ocr page(%d)======\n", i);
        FPDF_PAGE page = FPDF_LoadPage(mDocument, i);
        int pwidth = FPDF_GetPageWidth(page);
        int pheight = FPDF_GetPageHeight(page);
        float sample_size = std::min(max_size/pwidth, max_size/pheight);
        int width = (int)(sample_size*pwidth);
        int height = (int)(sample_size*pheight);
        FPDF_BITMAP bitmap = FPDFBitmap_Create(width, height,0);
        FPDFBitmap_FillRect(bitmap,0 ,0 , width, height, 0xFFFFFFFF);
//        FPDF_BITMAP bitmap = FPDFBitmap_CreateEx(width, height, format, first_scan, width*stride);
//        FPDFBitmap_FillRect(bitmap, 0, 0, width, height, 0xFFFFFFff);
        FPDF_RenderPageBitmap(bitmap, page, 0,0, width, height, 0, flag);
        int stride = FPDFBitmap_GetStride(bitmap);
        printf("====pwidth(%d), pheight(%d), width(%d), height(%d)=====\n", pwidth,pheight, width, height);
        void* first_scan = FPDFBitmap_GetBuffer(bitmap);
        PIX *pixd = pixCreate(width, height, 8);
        pixSetResolution(pixd, width, height);
        l_uint32 *src = (l_uint32 *) first_scan;
        l_int32 srcWpl = (stride / 4);
        l_uint32 *dst = pixGetData(pixd);
        l_int32 dstWpl = pixGetWpl(pixd);
        l_uint8 a, r, g, b, pixel8;

        for (int y = 0; y < height; y++) {
            l_uint32 *dst_line = dst + (y * dstWpl);
            l_uint32 *src_line = src + (y * srcWpl);

            for (int x = 0; x < width; x++) {
                // Get pixel from RGBA_8888
                r = *src_line >> SK_R32_SHIFT;
                g = *src_line >> SK_G32_SHIFT;
                a = *src_line >> SK_B32_SHIFT;
                b = *src_line >> SK_A32_SHIFT;
                pixel8 = (l_uint8)((r + g + b) / 3);

                // Set pixel to LUMA_8
                SET_DATA_BYTE(dst_line, x, pixel8);

                // Move to the next pixel
                src_line++;
            }
        }

        Ocr::get_instance().ocr(pixd);
        pixDestroy(&pixd);
        FPDFBitmap_Destroy(bitmap);
        FPDF_ClosePage(page);
    }
}

void PdfTessSource::open()
{
    mDocument = FPDF_LoadDocument(mPath, mPwd);
    unsigned long err = FPDF_GetLastError();
    if(err != FPDF_ERR_SUCCESS) {
        printf("====open doc err(%ld)====\n", err);
    }
}
