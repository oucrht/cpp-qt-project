#ifndef MEDIANFILTER_H
#define MEDIANFILTER_H

#include "Filter.h"
//#include <QObject>
class MedianFilter : public Filter
{
    Q_OBJECT
public:
    MedianFilter(int size);
     virtual ~MedianFilter();
    virtual Matrix Filtering(const Matrix &input); // 中值滤波器函数
    void _medianfilter(const Matrix *image, Matrix *result, int N, int M);
};

#endif // MEDIANFILTER_H
