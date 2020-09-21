#ifndef FILTER_H
#define FILTER_H

#include "image.h"
#include <QObject>

class Filter: public QObject
{
    Q_OBJECT
public:
    Filter(int size):filterSize(size)//构造函数
    {
    }

    virtual ~Filter(){}; //析构函数;
    virtual Matrix Filtering(const Matrix &input) = 0; //滤波函数（纯虚函数）;
protected:
    int filterSize;
};


#endif // FILTER_H

//作为均值滤波和中值滤波共同的基类
//抽象类
