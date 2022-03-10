#include "djvutesssource.h"

#include <stdio.h>
#include <string.h>

#include "leptonica/allheaders.h"

#include "ocr.h"

void DjVuTessSource::initDjVuEngine()
{

}

DjVuTessSource::DjVuTessSource(const char *path, const char *pwd):mPath(path), mPwd(pwd)
{
    printf("file:%s, pwd:%s\n", mPath, mPwd);
}

DjVuTessSource::~DjVuTessSource()
{
    if(mContext)
    {
        ddjvu_context_release(mContext);
        mContext = NULL;
    }
    if(mDocument)
    {
        ddjvu_document_release(mDocument);
        mDocument = NULL;
    }
    if(pixelformat)
    {
        ddjvu_format_release(pixelformat);
        pixelformat = NULL;
    }
}

void DjVuTessSource::ocr()
{
    open();
    printf("=======pagecount(%d)=======\n", mCount);
    for(int i=0;i<mCount;i++) {
        printf("======begin ocr page(%d)====\n", i);
        ddjvu_page_t *page = ddjvu_page_create_by_pageno(mDocument, i);
        while(!ddjvu_page_decoding_done(page)) {
            handle();
        }
        ddjvu_pageinfo_t t;
        if(ddjvu_document_get_pageinfo(mDocument, i, &t) == DDJVU_JOB_OK) {
            printf("pageinfo(width:%d, height:%d, dpi:%d, rotation:%d )\n", t.width, t.height, t.dpi, t.rotation);
            float max_size = 1280;
            float sample_size = std::min(max_size/t.width, max_size/t.height);
            int sample_width = (int)(sample_size * t.width);
            int sample_height = (int)(sample_size * t.height);
            ddjvu_rect_t pagerect;
            char *pixels = NULL;
            int stride = sample_width * 3;
            int size = stride * sample_height;
            pixels =(char*) malloc(size);

            /* fill pixel map with white color */
            memset(pixels, 0xFF, size);
            pagerect.x = 0;
            pagerect.y = 0;
            pagerect.w = sample_width;
            pagerect.h = sample_height;

            printf("renderrect(w : %d, h: %d)\n", pagerect.w, pagerect.h);

            if(ddjvu_page_render(page,
                                 DDJVU_RENDER_COLOR,
                                 &pagerect,
                                 &pagerect,
                                 pixelformat,
                                 stride,
                                 pixels)) {
                Ocr::get_instance().ocr((const unsigned char*)pixels, sample_width, sample_height, 3, stride, t.dpi);
            }
            free(pixels);
        }
        ddjvu_page_release(page);
    }
}

void DjVuTessSource::open()
{
    mContext = ddjvu_context_create("ddjvu_ocr");
    mDocument = ddjvu_document_create_by_filename(mContext, mPath, 1);
    while(!ddjvu_document_decoding_done(mDocument)) {
        handle();
    }
    mCount = ddjvu_document_get_pagenum(mDocument);
    pixelformat = ddjvu_format_create(DDJVU_FORMAT_RGB24, 0, NULL);
    ddjvu_format_set_row_order(pixelformat, 1);
    ddjvu_format_set_y_direction(pixelformat, 1);
    ddjvu_format_set_gamma(pixelformat, 2.2);
}

int DjVuTessSource::handle()
{
    const ddjvu_message_t *msg;
    if(!mContext) {
        return 0;
    }
    msg = ddjvu_message_wait(mContext);
    while((msg = ddjvu_message_peek(mContext)))
    {
        switch (msg->m_any.tag) {
            case DDJVU_ERROR:
                if(msg->m_error.filename) {
                    printf("ddjvu error: file(%s), message(%s), lineno(%d)\n", msg->m_error.filename, msg->m_error.message, msg->m_error.lineno);
                    return -1;
                }else {
                    printf("ddjvu error: %s\n", msg->m_error.message);
                    return -1;
                }
            break;
        default:
            break;
        }
        ddjvu_message_pop(mContext);
    }
    return 0;
}
