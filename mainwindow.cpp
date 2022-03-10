#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QDir>
#include <QFileDialog>
#include <QDebug>
#include <QTextCodec>

#include "tesssourcefactory.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "select pdf or image");
    QByteArray bytes = file.toLocal8Bit();
    const char* data = bytes.data();
    qDebug()<<"selected: " << data;
    TessSource* source = build(data);
    if(source) {
        source->ocr();
    }
}

