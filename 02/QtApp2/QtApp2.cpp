#include "QtApp2.h"
#include <qpainter.h>
#include <fstream>


//#include <wingdi.h>

#define WORD unsigned short
#define DWORD unsigned long
#define LONG int

using namespace std;

typedef struct tagBITMAPFILEHEADER {
    WORD     bfType;  //位图文件的类型，必须为0x424d 即BM两个字符的ASCII码
    DWORD    bfSize;  //位图文件的大小，以字节为单位 包括该14字节    
    WORD     bfReserved1; //位图文件保留字，暂时不用，一般为0         
    WORD     bfReserved2; //位图文件保留字，暂时不用，一般为0         
    DWORD    bfOffBits; //位图数据距文件头的偏移量，以字节为单位，即前三部分和
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD    biSize;            //本结构所占用字节数 40字节
    LONG     biWidth;           //位图的宽度，以像素为单位
    LONG     biHeight;          //位图的高度，以像素为单位
    WORD     biPlanes;          //目标设备的级别，必须为1
    WORD     biBitCount;        //每个像素所需的位数，必须是1（双色）、

//4（16色）、8（256色）或24（真彩色）之一
DWORD    biCompression;     //位图压缩类型，必须是 0（BI_RGB不压缩）、
                              //1（BI_RLE8压缩类型）
//2（BI_RLE压缩类型）之一
DWORD    biSizeImage;       //位图的大小，以字节为单位
LONG     biXPelsPerMeter;   //位图水平分辨率，每米像素数
LONG     biYPelsPerMeter;   //位图垂直分辨率，每米像素数
DWORD    biClrUsed;         //位图实际使用的颜色表中的颜色数
DWORD    biClrImportant;    //位图显示过程中重要的颜色数 
} BITMAPINFOHEADER;



QtApp2::QtApp2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}


QtApp2::~QtApp2()
{
    fclose(fp);//关闭文件
}

//一维转二维
void QtApp2::From1Dto2D(unsigned char** dst, unsigned char* src, int rows, int cols)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int lineByte = (cols * 24 + 31) / 32 * 4;
    for (i=0; i < rows; i++)
    {
        for (j=0; j < lineByte; j++)
        {
            dst[i][j] = src[k];
            k++;
        }
    }
}

void QtApp2::From2Dto1D(unsigned char* dst, unsigned char** src, int rows, int cols)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int lineByte = (cols * 24 + 31) / 32 * 4;
    for (i = 0; i < rows; i++)
    {
        for (j = 0; j < lineByte; j++)
        {
            dst[k] = src[i][j];
            k++;
        }
    }

}

void QtApp2::FlipImageUpDown(unsigned char** a, int rows, int cols)
{
    int i, j;
    int lineByte = (cols * 24 + 31) / 32 * 4;
    for (i = 0; i < rows / 2; i++)
        for (j = 0; j < lineByte; j++) {
            int t = a[i][j]; 
            a[i][j] = a[rows - i - 1][j]; 
            a[rows - i - 1][j] = t;
        }
}

void QtApp2::FlipImageLeftRight(unsigned char** a, int rows, int cols)
{
    int i, j;
    int lineByte = (cols * 24 + 31) / 32 * 4;
    for (i = 0; i < lineByte / 2; i++)
        for (j = 0; j < rows; j++) {
            int t = a[j][i];
            a[j][i] = a[j][lineByte - i - 1];
            a[j][lineByte - i - 1] = t;
        }
}

void QtApp2::smallerSize(unsigned char** dst)
{
    unsigned char** src = new unsigned char* [bmpHeight];
    int lineByte = (bmpWidth * 24 + 31) / 32 * 4;
    for (int i = 0; i < bmpHeight; i++)
    {
        src[i] = new unsigned char[lineByte];
    }
    From1Dto2D(src, p1d, bmpHeight, bmpWidth);
    int i, j;
    int m = 0;
    int n = 0;
    for (i = 0; i < bmpHeight; i++)
    {
        if (i % 2 == 0)//是奇数行
        {
            for (j = 0; j < lineByte; j++)
            {
                if (j % 2 == 0)//是奇数列
                {
                    dst[m][n] = src[i][j];
                    n++;
                }
            }
            m++;
            n = 0;
        }
    }
    for (int i = 0; i < bmpHeight; i++)
        delete[]src[i];
    delete[]src;
}

bool QtApp2::saveImg()
{
    BITMAPINFOHEADER bmiHdr; //定义信息头        
    bmiHdr.biSize = 40;
    bmiHdr.biWidth = bmpWidth;
    bmiHdr.biHeight = bmpHeight;
    bmiHdr.biPlanes = 1;
    bmiHdr.biBitCount = 24;
    bmiHdr.biCompression = 0;
    bmiHdr.biSizeImage = bmpWidth * bmpHeight * 3;
    bmiHdr.biXPelsPerMeter = 0;
    bmiHdr.biYPelsPerMeter = 0;
    bmiHdr.biClrUsed = 0;
    bmiHdr.biClrImportant = 0;

    char* fileNameTemp;
    QByteArray ba = fileName.replace("/", "\\\\").toLatin1();
    fileNameTemp = ba.data();
    FILE* fp = fopen(fileNameTemp, "wb");
    if (fp)
    {
        BITMAPFILEHEADER fheader = { 0 };
        fheader.bfType = 'M' << 8 | 'B';
        fheader.bfSize = 40 + 14 + bmiHdr.biSizeImage;
        fheader.bfOffBits = 40 + 14;
        fwrite(&fheader, 1, 14, fp);
        fwrite(&bmiHdr, 1, 40, fp);
        fwrite(p1d, 1, bmiHdr.biSizeImage, fp);
        fclose(fp);
        return true;
    }
    return false;
}


//显示图片
void QtApp2::showImg()
{
    
    QImage* tempImage = new QImage(p1d, bmpWidth, bmpHeight, 3 * bmpWidth, QImage::Format_RGB888);
    
    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
 
}

void QtApp2::on_pushButton_clicked()
{
   
    //获取文件名
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图像"), "", tr("Images(*.png *.bmp *.jpg)"));
    if (fileName.isEmpty())
        return;
    else
    {
        QImage img;
        if (!(img.load(fileName)))
        {
            QMessageBox::information(this, tr("打开图像失败"), tr("打开图像失败！"));
            return;
        }
        else {
            //转换获取到的文件名
            char* charFileName;
            QByteArray ba = fileName.replace("/", "\\\\").toLatin1();
            charFileName = ba.data();
            fp = fopen(charFileName, "rb");
            if (fp == 0)
                return;
            BITMAPFILEHEADER BmpHeader;
            fread(&BmpHeader.bfType, sizeof(BmpHeader.bfType), 1, fp);
            fread(&BmpHeader.bfSize, sizeof(BmpHeader.bfSize), 1, fp);
            fread(&BmpHeader.bfReserved1, sizeof(BmpHeader.bfReserved1), 1, fp);
            fread(&BmpHeader.bfReserved2, sizeof(BmpHeader.bfReserved2), 1, fp);
            fread(&BmpHeader.bfOffBits, sizeof(BmpHeader.bfOffBits), 1, fp);

            BITMAPINFOHEADER head;
            fread(&head.biSize,     sizeof(head.biSize), 1, fp);
            fread(&head.biWidth, sizeof(head.biWidth), 1, fp);
            fread(&head.biHeight, sizeof(head.biHeight), 1, fp);
            fread(&head.biPlanes, sizeof(head.biPlanes), 1, fp);
            fread(&head.biBitCount, sizeof(head.biBitCount), 1, fp);
            
            fseek(fp, sizeof(head.biCompression), 1);
            fseek(fp, sizeof(head.biSizeImage), 1);
            fseek(fp, sizeof(head.biXPelsPerMeter), 1);
            fseek(fp, sizeof(head.biYPelsPerMeter), 1);
            fseek(fp, sizeof(head.biClrUsed), 1);
            fseek(fp, sizeof(head.biClrImportant), 1);

            bmpWidth = head.biWidth;
            bmpHeight = head.biHeight;
            biBitCount = head.biBitCount;
            //定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
            int lineByte = (bmpWidth * biBitCount +31) / 32 * 4;
            //申请位图数据所需要的空间，读位图数据进内存
            p1d = new unsigned char[lineByte * bmpHeight];
            fread(p1d, 1, lineByte * bmpHeight, fp);

            showImg();
        }
        
    }
}

    

void QtApp2::on_pushButton3_clicked()
{
    unsigned char** p2d = new unsigned char* [bmpHeight];
    int lineByte = (bmpWidth * 24 + 31) / 32 * 4;
    for (int i = 0; i < bmpHeight; i++)
    {
        p2d[i] = new unsigned char[lineByte];
    }
    From1Dto2D(p2d, p1d, bmpHeight, bmpWidth);
    FlipImageUpDown(p2d, bmpHeight, bmpWidth);
    From2Dto1D(p1d, p2d, bmpHeight, bmpWidth);
    QImage* tempImage = new QImage(p1d, bmpWidth, bmpHeight, 3 * bmpWidth, QImage::Format_RGB888);
    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
    for (int i = 0; i < bmpHeight; i++)
        delete[]p2d[i];
    delete[]p2d;
}

//缩小图片按钮
void QtApp2::on_pushButton4_clicked()
{
    unsigned char** p2d = new unsigned char* [bmpHeight];
    int lineByte = (bmpWidth * 24 + 31) / 32 * 4;
    for (int i = 0; i < bmpHeight; i++)
    {
        p2d[i] = new unsigned char[lineByte];
    }
    smallerSize(p2d);
    From2Dto1D(p1d, p2d, bmpHeight, bmpWidth);
    QImage* tempImage = new QImage(p1d, bmpWidth, bmpHeight, 3 * bmpWidth, QImage::Format_RGB888);

    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
    for (int i = 0; i < bmpHeight; i++)
        delete[]p2d[i];
    delete[]p2d;
}

//保存按钮
void QtApp2::on_pushButton5_clicked()
{
    if (saveImg())
    {
        QMessageBox::information(this, "提示", "保存成功", QMessageBox::Yes);
    }
    else {
        QMessageBox::critical(this, "提示", "保存失败", QMessageBox::Yes);
    }

}

//左右翻转按钮
void QtApp2::on_pushButton2_clicked()
{
    unsigned char** p2d = new unsigned char* [bmpHeight];
    int lineByte = (bmpWidth * 24 + 31) / 32 * 4;
    for (int i = 0; i < bmpHeight; i++)
    {
        p2d[i] = new unsigned char[lineByte];
    }
    From1Dto2D(p2d, p1d, bmpHeight, bmpWidth);
    FlipImageLeftRight(p2d, bmpHeight, bmpWidth);
    From2Dto1D(p1d, p2d, bmpHeight, bmpWidth);
    QImage* tempImage = new QImage(p1d, bmpWidth, bmpHeight, 3 * bmpWidth, QImage::Format_RGB888);
    
    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
    for (int i = 0; i < bmpHeight; i++)
        delete[]p2d[i];
    delete[]p2d;
}


