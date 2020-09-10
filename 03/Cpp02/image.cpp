#include "image.h"
#include <QDebug>
#include <math.h>
#define WORD unsigned short
#define DWORD unsigned long
#define LONG int
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

//Image::Image(QObject *parent) : QObject(parent)
//{

//}
//构造函数
Image::Image()
{
    data = new unsigned char* [0];
    for (int i = 0; i < 0; i++)
    {
        data[i] = new unsigned char[0];
    }

}
//构造函数重载
Image::Image(int h, int w)
{
    data = new unsigned char* [h];
    for (int i = 0; i < h; i++)
    {
        data[i] = new unsigned char[(w * 8 + 31) / 32 * 4];
    }

}
// 其他重载构造函数的实现
//构造函数重载，创建的图像像素值都为val
Image::Image(int h, int w, unsigned char val)
{
    data = new unsigned char* [h];
    for (int i = 0; i < h; i++)
    {
        data[i] = new unsigned char[(w * 8 + 31) / 32 * 4];
    }
    int i,j;
    for(i=0;i<h;i++)
    {
        for(j=0;j<(w * 8 + 31) / 32 * 4;j++)
        {
            data[i][j]=val;
        }
    }
}

//构造函数重载，利用文件名从硬盘加载图像文件成为 Image 对象
Image::Image(char* ImageName)
{
    //开辟data的内存空间需要先知道图像的长和宽
    //而要想知道图像的长和宽必须先打开图片，
    //因此data空间的开辟应当是在读取图片之后
    int i=0,j=0,k=0;
    Read(ImageName);
    data = new unsigned char* [height];
    for (int i = 0; i < height; i++)
    {
        data[i] = new unsigned char[(width * 8 + 31) / 32 * 4];
    }

    for (i=0; i < height; i++)
    {
        for (j=0; j < (width * 8 + 31) / 32 * 4; j++)
        {
            data[i][j] = img1d[k];
            k++;
        }
    }
}
//构造函数重载，从一维静态数组创建 Image 对象，图像的行数和列数由后面两个参数给出
//要求一维静态数组是8位格式
Image::Image(unsigned char* m, int rows, int cols)
{
    this->height=rows;
    this->width=cols;
    data = new unsigned char* [rows];
    for (int i = 0; i < rows; i++)
    {
        data[i] = new unsigned char[(cols * 8 + 31) / 32 * 4];
    }
    int i=0,j=0,k=0;
    for (i=0; i < rows; i++)
    {
        for (j=0; j < (cols * 8 + 31) / 32 * 4; j++)
        {
            data[i][j] = m[k];
            k++;
        }
    }
}
//构造函数重载，从动态数组（二级指针）创建 Image 对象，图像的行数和列数由后面两个参数给出
Image:: Image(unsigned char** m, int h, int w)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int lineSize = (w * 8 + 31) / 32 * 4;
    this->height=h;
    this->width=w;
    unsigned char* temp = new unsigned char[h*lineSize];
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < lineSize; j++)
        {
            temp[k] = m[i][j];
            k++;
        }
    }
    for (i=0; i < h; i++)
    {
        for (j=0; j < (w * 8 + 31) / 32 * 4; j++)
        {
            data[i][j] = temp[k];
            k++;
        }
    }
}

//拷贝构造函数
Image::Image(const Image& im)
{
    this->height=im.height;
    this->width=im.width;

    data = new unsigned char* [height];
    for (int i = 0; i < height; i++)
    {
        data[i] = new unsigned char[(width * 8 + 31) / 32 * 4];
    }
    this->data = im.data;

}
//析构函数
Image::~Image()
{
    for (int i = 0; i <height; i++)
        delete[]data[i];
    delete[]data;

    delete[]img1d;
    qDebug()<<"正在调用Image的析构函数";
}
//从硬盘读入图像文件;
void Image::Read(char* ImageName)
{
    qDebug()<<"成功传入Read函数："<<ImageName;
    FILE* fp = fopen(ImageName, "rb");
    if (fp == 0)
    {
        qDebug()<<"打开文件失败";
        return;
    }
    qDebug()<<"文件打开成功，正在进行读取...";
    BITMAPFILEHEADER BmpHeader;
    fread(&BmpHeader.bfType, sizeof(BmpHeader.bfType), 1, fp);
    fread(&BmpHeader.bfSize, sizeof(BmpHeader.bfSize), 1, fp);
    fread(&BmpHeader.bfReserved1, sizeof(BmpHeader.bfReserved1), 1, fp);
    fread(&BmpHeader.bfReserved2, sizeof(BmpHeader.bfReserved2), 1, fp);
    fread(&BmpHeader.bfOffBits, sizeof(BmpHeader.bfOffBits), 1, fp);

    BITMAPINFOHEADER head;
    fread(&head.biSize, sizeof(head.biSize), 1, fp);
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

    width = head.biWidth;
    height = head.biHeight;
    int biBitCount = head.biBitCount;
    //定义变量，计算图像每行像素所占的字节数（必须是4的倍数）
    int lineByte = (width * biBitCount + 31) / 32 * 4;
    //申请位图数据所需要的空间，读位图数据进内存
    unsigned char * src = new unsigned char[lineByte * height];
    fread(src, 1, lineByte * height, fp);
    qDebug()<<"文件读取完成，正在转灰度图...";

    img1d = new unsigned char[((width * 8 + 31) / 32 * 4)*height];
    //转灰度
    int i = 0;
    int j = 0;
    int k = 0;
    int temp=0;

    for(i=0;i<lineByte*height;i+=3)
    {
        temp =0;
        for(j=0;j<3;j++)
        {
            temp+=src[i+j];
        }
        img1d[k]=(unsigned char)(temp/3);
        k++;

    }

}
//获取图像中指定点的值
unsigned char& Image::At(int row, int col)
{

    return data[row][col];
}
//设置图像为同一值
void Image::Set(unsigned char value)
{
    int lineByte = (width * 8 + 31) / 32 * 4;
    int i,j;
    for(i=0;i<height;i++)
    {
        for(j=0;j<lineByte*height;j++)
        {
            data[i][j]=value;
        }
    }
    from2dto1d(data,img1d);
}
//0 左右，1 上下;
void Image::Flip(int code)
{
    int lineByte =(width * 8 + 31) / 32 * 4;
    int i, j;
    if(code == 1)
    {

        for (i = 0; i < height / 2; i++)
        {
            for (j = 0; j < lineByte; j++)
            {
                unsigned char t = data[i][j];
                data[i][j] = data[height - i - 1][j];
                data[height - i - 1][j] = t;
            }
        }
        from2dto1d(data,img1d);
    }
    if(code == 0)
    {
        //int k=0,m;
        for (i = 0; i < height; i++)
        {
            for (j = 0; j < lineByte/2; j++)
            {
                unsigned char t=data[i][j];
                data[i][j]=data[i][lineByte-j-1];
                data[i][lineByte-j-1] = t;
            }

        }
        from2dto1d(data,img1d);
    }
    else
        return;

}
//图像缩小，放大,0缩小，1放大
void Image::Resize(int code)
{
    if(code == 0)
    {
        int i, j,k;
        int m = 0;
        int lineByte =(width * 8 + 31) / 32 * 4;
        unsigned char ** dst = new unsigned char* [height];
        for (int i = 0; i < height; i++)
        {
            dst[i] = new unsigned char[(width * 8 + 31) / 32 * 4];
        }
        for (i = 0; i < height; i+=2)
        {
            k = 0;
            for (j = 0; j < lineByte; j+=2)
            {

                dst[m][k] = data[i][j];
                k++;
            }
            m++;
        }

        if (width % 2 == 0)
        {
            width = width / 2;
        }
        else
        {
            width = width / 2 + 1;
        }
        if (height % 2 == 0)
        {
            height =height / 2;
        }
        else
        {
            height = height / 2 + 1;
        }
        data =dst;
        from2dto1d(data,img1d);
        for (int i = 0; i < height; i++)
            delete[]dst[i];
        delete[]dst;
    }

}
//图像裁剪的函数
//裁剪点(x1,y1)到点(x2,y2)的图像
void Image::Cut(int x1, int y1, int x2, int y2)
{
    int i,j,m,n,temp;
    height = abs(y1-y2);
    width =abs(x1-x2);
    unsigned char ** dst = new unsigned char* [height];
    for (int i = 0; i < height; i++)
    {
        dst[i] = new unsigned char[(width * 8 + 31) / 32 * 4];
    }
    if(x1<x2) m=x1;
    else m=x2;
    if(y1<y2) n=y1;
    else n = y2;
    temp = m;//保存m的值
    for(i=0;i<height;i++)
    {
        for(j=0,m=temp;j<(width * 8 + 31) / 32 * 4;j++)
        {
            dst[i][j]=data[n][m];
            m++;
        }
        n++;
    }
    data = dst;

    from2dto1d(data,img1d);
    for (int i = 0; i < height; i++)
        delete[]dst[i];
    delete[]dst;
}

//图像旋转的函数
 void Image::Rotate(int degree)
 {

     unsigned char ** dst = new unsigned char* [width];
     for (int i = 0; i < width; i++)
     {
         dst[i] = new unsigned char[(height * 8 + 31) / 32 * 4];
     }
     int i,j;
     for(i=0;i<width;i++)
     {
         for(j=0;j<height;j++)
         {
             *(*(dst+i)+j) = *(*(data+j)+i);
         }
     }
     data = dst;
     from2dto1d(data,img1d);

 }
//求图像的均值和方差，利用参数输出
 void Image::Mean_Variance(float& m, float& var)
 {
     int i,j;
     float sum=0;
     for(i=0;i<height;i++)
     {
         for(j=0;j<(width * 8 + 31) / 32 * 4;j++)
         {
             sum+=data[i][j];
         }
     }
     m=sum/(height*((width * 8 + 31) / 32 * 4));

     sum=0;
     for(i=0;i<height;i++)
     {
         for(j=0;j<(width * 8 + 31) / 32 * 4;j++)
         {
            sum+=pow(data[i][j]-m,2);
         }
     }
     var = sum/(height*((width * 8 + 31) / 32 * 4));
 }
//实现友元函数，交换两个 Image 对象的数据
void Swap(Image& a, Image& b)
{
}

int Image::getHeight()
{
    return height;
}

int Image::getWidth()
{
    return width;
}

unsigned char * Image::getImgPointer()
{
    return img1d;
}

void Image::from2dto1d(unsigned char** src, unsigned char * dst)
{
    int i = 0;
    int j = 0;
    int k = 0;
    int lineSize = (width * 8 + 31) / 32 * 4;
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < lineSize; j++)
        {
            dst[k] = src[i][j];
            k++;
        }
    }
    img1d = dst;

}
