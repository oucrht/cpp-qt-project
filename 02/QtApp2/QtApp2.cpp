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




void QtApp2::showImg()
{
    /*
    unsigned char** p2d = new unsigned char *[bmpHeight];
    int lineByte = (bmpWidth * 24 + 31) / 32 * 4;
    for (int i = 0; i < bmpHeight; i++)
    {
        p2d[i] = new unsigned char[lineByte];
    }
    From1Dto2D(p2d, p1d, bmpHeight, bmpWidth);
    FlipImageUpDown(p2d, bmpHeight, bmpWidth);
    From2Dto1D(p1d, p2d, bmpHeight, bmpWidth);
    */
    QImage* tempImage = new QImage(p1d, bmpWidth, bmpHeight, 3 * bmpWidth, QImage::Format_RGB888);
    //QMatrix matrix;
    //matrix.rotate(90);
    //* tempImage = tempImage->transformed(matrix);
    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
    /*
    for (int i = 0; i < bmpHeight; i++)
        delete[]p2d[i];
    delete[]p2d;
    */
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
           /*
            ui.label->setText(fileName.replace("/","\\\\"));
            ifstream inFile(fileName.replace("/","\\\\").toStdString(), ios::in | ios::binary);
            if (!inFile)
            {
                QMessageBox::critical(this, "Error", "打开文件失败");
                 return;
            }
            while (inFile.read((char*)&test, 16 * sizeof(char)))
            {
                mybfType = test.bfType;
            }
            mybfType = mybfSize / (1024);
            ui.label->setText(QString::number(mybfType,2));
            */
            char* charFileName;
            QByteArray ba = fileName.replace("/", "\\\\").toLatin1();
            charFileName = ba.data();
          //  strcpy(charFileName, fileName.replace("/", "\\\\").toStdString().c_str());
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
            int lineByte = (bmpWidth * biBitCount +31) / 32 * 4;//灰度图像有颜色表，且颜色表表项为25
            //申请位图数据所需要的空间，读位图数据进内存
            p1d = new unsigned char[lineByte * bmpHeight];
            fread(p1d, 1, lineByte * bmpHeight, fp);
            //fclose(fp);//关闭文件

            showImg();

            //unsigned char B=p1d[0];
            //unsigned char G=p1d[1];
            //unsigned char R=p1d[2];
            //ui.label->setPixmap()

            //ui.label_3->setText(QString::number(bmpWidth));
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
    //QMatrix matrix;
    //matrix.rotate(90);
    //* tempImage = tempImage->transformed(matrix);
    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
    for (int i = 0; i < bmpHeight; i++)
        delete[]p2d[i];
    delete[]p2d;
}

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
    //QMatrix matrix;
    //matrix.rotate(90);
    //* tempImage = tempImage->transformed(matrix);
    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
    for (int i = 0; i < bmpHeight; i++)
        delete[]p2d[i];
    delete[]p2d;
}


