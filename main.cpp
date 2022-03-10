//#include "mainwindow.h"

//#include <QApplication>
//#include <QTextCodec>
#include "pdftesssource.h"
#include "djvutesssource.h"
#include "tesssourcefactory.h"
int main(int argc, char *argv[])
{
    PdfTessSource::initPdfEngine();
    DjVuTessSource::initDjVuEngine();
//    const char* path = "/home/weikano/文档/需要的文件/书籍/code.pdf";
    const char* path = "/home/weikano/文档/需要的文件/书籍/DjVu3Spec.djvu";
    TessSource* source = build(path);
    if(source) {
        source->ocr();
    }
//    TessSource* source = new PdfTessSource("/home/weikano/文档/需要的文件/书籍/code.pdf", "");
//    TessSource* source = new DjVuTessSource("/home/weikano/文档/需要的文件/书籍/code.pdf", "");

    return 0;
//    QApplication a(argc, argv);
//    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

//    MainWindow w;
//    w.show();
//    return a.exec();
}
