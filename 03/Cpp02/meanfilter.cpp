#include "meanfilter.h"
#include <QDebug>

MeanFilter::MeanFilter(int size):Filter(size)
{

}

MeanFilter::~MeanFilter()
{

}

Matrix MeanFilter::Filtering(const Matrix &input)
{
    int h,w;
    h=input.Height();//获取高度
    w=input.Width();//获取宽度

    qDebug()<<"h="<<h<<"w="<<w;

    const Matrix output(h,w);

    int* xLocation = new int[filterSize];
    int* yLocation = new int[filterSize];
    double sum;
    int index;

    for (int i = filterSize / 2; i<h - filterSize / 2; i++)
    {
        xLocation[filterSize / 2] = i;
        for (int delta = 0; delta <= filterSize / 2; delta++)
        {
            xLocation[filterSize / 2 - delta] = i - delta;
            xLocation[filterSize / 2 + delta] = i + delta;
        }

        for (int j = filterSize / 2; j<w - filterSize / 2; j++)
        {
            yLocation[filterSize / 2] = j;
            for (int delta = 0; delta <= filterSize / 2; delta++)
            {
                yLocation[filterSize / 2 - delta] = j - delta;
                yLocation[filterSize / 2 + delta] = j + delta;
            }

            sum = 0;
            for (int fi = 0; fi < filterSize; fi++)
            {
                for (int fj = 0; fj < filterSize; fj++)
                {
                    index = fi*filterSize + fj;
                    sum += input.At(xLocation[fi], yLocation[fj]);
                }
            }
            output.Set(i,j,sum / (filterSize * filterSize));  //filterSize*filterSize为模板面积
        }
    }
    qDebug()<<"meanfilter函数执行结束";

    return output;
}
