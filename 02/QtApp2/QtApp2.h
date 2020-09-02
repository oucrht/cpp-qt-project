#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtApp2.h"
#include <QImage>
#include <QPixmap>
#include <QFileDialog>
#include <QMessageBox>
#include <QScreen>
#include <QGuiAPplication>

#include <string>
using namespace std;
class QtApp2 : public QMainWindow
{
    Q_OBJECT

public:
    QtApp2(QWidget *parent = Q_NULLPTR);
    ~QtApp2();
    void From1Dto2D(unsigned char** dst, unsigned char* src, int rows, int cols);
    void From2Dto1D(unsigned char* dst, unsigned char** src, int rows, int cols);
    void FlipImageUpDown(unsigned char** a, int rows, int cols);
    void FlipImageLeftRight(unsigned char** a, int rows, int cols);
    void smallerSize(unsigned char **dst);
    bool saveImg();
    void showImg();

private slots:
    void on_pushButton_clicked();
    void on_pushButton2_clicked();
    void on_pushButton3_clicked();
    void on_pushButton4_clicked();
    void on_pushButton5_clicked();

private:
    Ui::QtApp2Class ui;
    QString fileName;
    FILE* fp;
    int bmpWidth;
    int bmpHeight;
    //int lineByte;
    int biBitCount;//图像类型，每像素位数
    unsigned char* p1d;//读入图像数据的指针
};
