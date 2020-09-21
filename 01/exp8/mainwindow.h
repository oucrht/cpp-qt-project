#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void paintEvent(QPaintEvent*);



private:
    Ui::MainWindow *ui;
    int tran;
    int A = 1;//A用来记录while循环了几轮
    double m = 10.0, pi2, z;
    double Q;//判断是否在圆内
    double e = 0.01;
    double *x, *y;
    double pi1 = 3;
    int numIn=0;
    int numAll=0;
};
#endif // MAINWINDOW_H
