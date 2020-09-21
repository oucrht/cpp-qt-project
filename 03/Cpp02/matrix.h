#ifndef MATRIX_H
#define MATRIX_H

//#include <QObject>

class Matrix
{

public:
    Matrix();
    Matrix(int h,int w);//(done)
    Matrix(int h, int w, double val);//(done)
    //Matrix(const Matrix &m);
    virtual ~Matrix();
    void Zeros(int h, int w); // 根据参数产生 h 行 w 列的全零矩阵
    void Ones(int h, int w); // 根据参数产生 h 行 w 列的全 1 矩阵
    void Random(int h, int w); //产生 h 行 w 列的随机矩阵，矩阵的元素为 [0,1]之间的随机实数（double 类型）
    void Identity(int n); // 根据参数产生 n 行 n 列的单位矩阵
    int Height(); // 获得矩阵的行数(done)
    int Width(); // 获得矩阵的列数(done)

    int Height() const;
    int Width() const;
    Matrix MajorDiagonal();//(done) 求矩阵主对角线上的元素，输出一个 N 行 1 列的矩阵，N 为主对角线元素的个数
    Matrix MinorDiagonal();// 求矩阵的副对角线上的元素，输出一个 N 行 1 列的矩阵，N 为副对角线上元素的个数
    Matrix Row(int n);// 返回矩阵的第 n 行上的元素，组出一个 1 行 N 列的矩阵输出，N 为第 n 行上元素的个数
    Matrix Column(int n);// 返回矩阵的第 n 列上的元素，组出一个 N 行 1 列的矩阵输出，N 为第 n 列上元素的个数

    void Transpose(); // 将矩阵转置
    double& At(int row, int col); //(done)获取第 row 行第 col 列的矩阵元素的值
    double& At(int row, int col) const; //(done)获取第 row 行第 col 列的矩阵元素的值
    void Set(int row, int col, double value); //(done)设置第 row 行第 col 列矩阵元素的值为 value
    void Set(double value); //(done)设置矩阵所有元素为同一值 value
    void Set(int row, int col, double value) const;
    void Normalize(); // 该函数把矩阵的数据线性缩放至[0,1]区间，即把当前矩阵所有元素中的最小值 min 变成 0，最大值 max 变为 1，其他元素的值线性变到[0,1]区间，公式为：t’=(t-min)/max;
    void Reshape(int h, int w); //在矩阵元素总数不变的情况下，将矩阵行列变为参数给定的大小
    bool IsEmpty();// 判断矩阵是否为空矩阵
    bool IsSquare();// 判断矩阵是否为方阵
    void CopyTo(Matrix &m); // 将矩阵复制给 m
    void Mul(double s); // 矩阵的每个元素都乘以参数 s
    void Cat(Matrix &m, int code); // 将矩阵 m 与当前矩阵进行拼接，code代表拼接的方式：将 m 拼接到当前矩阵的上、下、左、右，具体例子见后面的说明
    friend Matrix Add(const Matrix &m1, const Matrix &m2); // 友元函数，将矩阵 m1 和 m2 相加，结果矩阵作为函数的返回值
    friend Matrix Sub(const Matrix &m1, const Matrix &m2); // 友元函数，将矩阵 m1 和 m2 相减，结果矩阵作为函数的返回值
    friend void Swap(Matrix &a, Matrix &b); // 友元函数，交换两个矩阵




    int height;
    int width;
    double **data;
    double *dt1;
};

#endif // MATRIX_H
