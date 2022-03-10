#ifndef TESSSOURCEFACTORY_H
#define TESSSOURCEFACTORY_H
#include <string>
#include "imagefiletesssource.h"
#include "pdftesssource.h"
#include "djvutesssource.h"

static inline bool ends_with(const char* value, const std::string& ending)
{
    std::string svalue(value);
    if(ending.size()>svalue.length()) {
        return false;
    }
    return std::equal(ending.rbegin(), ending.rend(), svalue.rbegin());
}

TessSource* build(const char* url)
{
    if(ends_with(url, ".png") || ends_with(url, ".jpg") || ends_with(url, ".jpeg")) {
        return new ImageFileTessSource(url);
    }
    else if(ends_with(url, ".pdf")) {
        return new PdfTessSource(url, "");
    }else if(ends_with(url, ".djvu")) {
        return new DjVuTessSource(url, "");
    } else {
        return nullptr;
    }
}

#endif // TESSSOURCEFACTORY_H
