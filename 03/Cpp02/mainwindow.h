#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "meanfilter.h"
#include "medianfilter.h"
#include "image.h"
#include "mylabel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void openFile();
    void flipUD();
    void flipRL();
    void resizeImg();
    void showInfo(int x,int y);
    void openDialog();
    void getCutAera(int x1,int y1,int x2, int y2);
    void rotato();
    void showMean();

    void meanFilter();
    void medianFilter();

private:
    Ui::MainWindow *ui;
    Image *myImg;
    myLabel *output;
    myLabel *val;
};
#endif // MAINWINDOW_H
