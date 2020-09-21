#include "medianfilter.h"


MedianFilter::MedianFilter(int size):Filter(size)
{

}

MedianFilter::~MedianFilter()
{

}

void swap(uchar &a, uchar &b)
{
    uchar t;
    t = a;
    a = b;
    b = t;
}

uchar Median(uchar n1, uchar n2, uchar n3, uchar n4, uchar n5,
    uchar n6, uchar n7, uchar n8, uchar n9) {
    uchar arr[9];
    arr[0] = n1;
    arr[1] = n2;
    arr[2] = n3;
    arr[3] = n4;
    arr[4] = n5;
    arr[5] = n6;
    arr[6] = n7;
    arr[7] = n8;
    arr[8] = n9;
    for (int gap = 9 / 2; gap > 0; gap /= 2)
        for (int i = gap; i < 9; ++i)
            for (int j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap)
                swap(arr[j], arr[j + gap]);
    return arr[4];//返回中值
}


Matrix MedianFilter::Filtering(const Matrix &input)
{
    int h,w;
    h = input.Height();
    w = input.Width();

    const Matrix output(h,w);
    for(int i=0;i<h;++i)
            for (int j=0; j < w; ++j) {
                if ((i - 1) > 0 && (i + 1) < h && (j - 1) > 0 && (j + 1) < w) {
                    output.Set(i, j ,Median(input.At(i, j), input.At(i + 1, j + 1),
                        input.At(i + 1, j), input.At(i, j + 1), input.At(i + 1, j - 1),
                        input.At(i - 1, j + 1),input.At(i - 1, j), input.At(i, j - 1),
                        input.At(i - 1, j - 1)));

                }
                else
                    output.Set(i, j ,input.At(i, j));
            }




    return output;
}
