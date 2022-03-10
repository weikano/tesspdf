#ifndef TESSSOURCE_H
#define TESSSOURCE_H

#include <string>
/**
 * @brief The TessSource class
 * tesseract ocr data source
 */
class TessSource
{
public:
    TessSource();
    virtual ~TessSource() = 0;
    virtual void ocr() = 0;
};

#endif // TESSSOURCE_H
