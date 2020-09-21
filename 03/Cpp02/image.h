#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include "matrix.h"

class Image : public QObject, public Matrix
{
    Q_OBJECT
public:
    //explicit Image(QObject *parent = nullptr);
    Image(); //无参数的构造函数，创建行列都为零的 Image 对象
    Image(int h, int w); //构造函数重载，创建 h 行，w 列的 Image 对象
    Image(int h, int w, unsigned char val); //构造函数重载，创建的图像像素值都为val
    Image(char* ImageName); //构造函数重载，利用文件名从硬盘加载图像文件成为 Image 对象
    Image(unsigned char* m, int rows, int cols); //构造函数重载，从一维静态数组创建 Image 对象，图像的行数和列数由后面两个参数给出
    Image(unsigned char** m, int h, int w); //构造函数重载，从动态数组（二级指针）创建 Image 对象，图像的行数和列数由后面两个参数给出
    Image(const Image& im); //拷贝构造函数（深拷贝）
    ~Image(); //析构函数
    void Read(char* ImageName); //从硬盘文件中读入图像数据
    unsigned char& At(int row, int col); //获取第 row 行第 col 列的像素点的值
    void Set(int row, int col, unsigned char value); //设置像素(row,col)为某值
    void Set(unsigned char value); //设置图像所有像素为同一值
    void Flip(int code); //图像的翻转; 根据 code 的值：0:左右翻转，1:上下翻转
    void Resize(int code); //图像的缩放;根据 code 的值：0:缩小一倍，1:放大一倍
    void Cut(int x1, int y1, int x2, int y2);//裁剪点(x1,y1)到点(x2,y2)的图像
    void Rotate(int degree);//图像旋转的函数（简单起见，旋转角度为 90 度的整数倍）
    void Mean_Variance(float& m, float& var);//求图像的均值和方差，利用参数输出
    friend void Swap(Image& a, Image& b);//使用友元函数交换两个 Image 对象的数据

    int getHeight();
    int getWidth();
    unsigned char * getImgPointer();
    void from2dto1d(unsigned char** src, unsigned char * dst);
    void from2dto1d();
    //运算符重载

    Image& operator=(const Matrix& mat);



signals:
private:
    unsigned char** data;
    unsigned char * img1d;
    int height;
    int width;


};

#endif // IMAGE_H
