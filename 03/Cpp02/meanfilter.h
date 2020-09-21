#ifndef MEANFILTER_H
#define MEANFILTER_H

#include "Filter.h"
//#include <QObject>

class MeanFilter : public Filter
{
    Q_OBJECT
public:
    MeanFilter(int size);
    virtual ~MeanFilter();
    virtual Matrix Filtering(const Matrix &input); //均值滤波函数
};

#endif // MEANFILTER_H

