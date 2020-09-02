#include "QtApp2.h"
#include <qpainter.h>
#include <fstream>


//#include <wingdi.h>

#define WORD unsigned short
#define DWORD unsigned long
#define LONG int

using namespace std;

typedef struct tagBITMAPFILEHEADER {
    WORD     bfType;  //λͼ�ļ������ͣ�����Ϊ0x424d ��BM�����ַ���ASCII��
    DWORD    bfSize;  //λͼ�ļ��Ĵ�С�����ֽ�Ϊ��λ ������14�ֽ�    
    WORD     bfReserved1; //λͼ�ļ������֣���ʱ���ã�һ��Ϊ0         
    WORD     bfReserved2; //λͼ�ļ������֣���ʱ���ã�һ��Ϊ0         
    DWORD    bfOffBits; //λͼ���ݾ��ļ�ͷ��ƫ���������ֽ�Ϊ��λ����ǰ�����ֺ�
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD    biSize;            //���ṹ��ռ���ֽ��� 40�ֽ�
    LONG     biWidth;           //λͼ�Ŀ�ȣ�������Ϊ��λ
    LONG     biHeight;          //λͼ�ĸ߶ȣ�������Ϊ��λ
    WORD     biPlanes;          //Ŀ���豸�ļ��𣬱���Ϊ1
    WORD     biBitCount;        //ÿ�����������λ����������1��˫ɫ����

//4��16ɫ����8��256ɫ����24�����ɫ��֮һ
DWORD    biCompression;     //λͼѹ�����ͣ������� 0��BI_RGB��ѹ������
                              //1��BI_RLE8ѹ�����ͣ�
//2��BI_RLEѹ�����ͣ�֮һ
DWORD    biSizeImage;       //λͼ�Ĵ�С�����ֽ�Ϊ��λ
LONG     biXPelsPerMeter;   //λͼˮƽ�ֱ��ʣ�ÿ��������
LONG     biYPelsPerMeter;   //λͼ��ֱ�ֱ��ʣ�ÿ��������
DWORD    biClrUsed;         //λͼʵ��ʹ�õ���ɫ���е���ɫ��
DWORD    biClrImportant;    //λͼ��ʾ��������Ҫ����ɫ�� 
} BITMAPINFOHEADER;



QtApp2::QtApp2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}


QtApp2::~QtApp2()
{
    fclose(fp);//�ر��ļ�
}

//һάת��ά
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
        if (i % 2 == 0)//��������
        {
            for (j = 0; j < lineByte; j++)
            {
                if (j % 2 == 0)//��������
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
    BITMAPINFOHEADER bmiHdr; //������Ϣͷ        
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


//��ʾͼƬ
void QtApp2::showImg()
{
    
    QImage* tempImage = new QImage(p1d, bmpWidth, bmpHeight, 3 * bmpWidth, QImage::Format_RGB888);
    
    ui.label->setPixmap(QPixmap::fromImage(*tempImage));
 
}

void QtApp2::on_pushButton_clicked()
{
   
    //��ȡ�ļ���
    QString fileName = QFileDialog::getOpenFileName(this, tr("ѡ��ͼ��"), "", tr("Images(*.png *.bmp *.jpg)"));
    if (fileName.isEmpty())
        return;
    else
    {
        QImage img;
        if (!(img.load(fileName)))
        {
            QMessageBox::information(this, tr("��ͼ��ʧ��"), tr("��ͼ��ʧ�ܣ�"));
            return;
        }
        else {
            //ת����ȡ�����ļ���
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
            //�������������ͼ��ÿ��������ռ���ֽ�����������4�ı�����
            int lineByte = (bmpWidth * biBitCount +31) / 32 * 4;
            //����λͼ��������Ҫ�Ŀռ䣬��λͼ���ݽ��ڴ�
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

//��СͼƬ��ť
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

//���水ť
void QtApp2::on_pushButton5_clicked()
{
    if (saveImg())
    {
        QMessageBox::information(this, "��ʾ", "����ɹ�", QMessageBox::Yes);
    }
    else {
        QMessageBox::critical(this, "��ʾ", "����ʧ��", QMessageBox::Yes);
    }

}

//���ҷ�ת��ť
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


