#include "matrix.h"
#include <QDebug>
Matrix::Matrix()
{

}

Matrix::Matrix(int h, int w)
{
    this->height= h;
    this->width=w;
    data =new double * [h];
    for (int i = 0; i < h; i++)
    {
        data[i] =new double [(w * 8 + 31) / 32 * 4];
    }
}

Matrix::Matrix(int h, int w, double val)
{
    int i,j;
    data =new double * [h];
    for (i = 0; i < h; i++)
    {
        data[i] =new double [(w * 8 + 31) / 32 * 4];
    }
    for(i = 0;i<h;i++)
    {
        for(j=0;j<w;j++)
        {
            data[i][j]=val;
        }
    }
}

Matrix::~Matrix()
{
    //TODO 此处有BUG，当去掉下面的注释后会出现程序崩溃。
//    qDebug()<<"正在调用Matrix的析构函数";
//    for (int i = 0; i <height; i++)
//        delete[]data[i];
//    delete[]data;
    qDebug()<<"Matrix析构析构函数调用结束";
}

int Matrix::Height()
{
    return height;
}

int Matrix::Width()
{
    return width;
}

int Matrix::Width() const
{
    return width;
}

int Matrix::Height() const
{
    return height;
}
Matrix Matrix::MajorDiagonal()
{

    int i;
    if(height <= width)
    {
        Matrix dst(height,1);
        for(i=0;i<height;i++)
        {
            dst.data[i][0] = data[i][i];
        }
        return dst;
    }
    else
    {
        Matrix dst(width,1);
        for(i=0;i<width;i++)
        {
            dst.data[i][0] = data[i][i];
        }
        return dst;
    }

}

double &Matrix::At(int row, int col)
{
    return data[row][col];
}

double &Matrix::At(int row, int col) const
{
    return data[row][col];
}

void Matrix::Set(int row, int col, double value)
{
    data[row][col] = value;
}

void Matrix::Set(double value)
{
    int i,j;
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            data[i][j]=value;
        }
    }
}

void Matrix::Set(int row, int col, double value) const
{
    data[row][col] = value;
}


