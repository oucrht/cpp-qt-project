#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QImage>
#include <QLabel>
#include <QDebug>
#include <QInputDialog>
#include "dialog.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //设置状态栏
    QLabel *statusLabel = new QLabel();
    statusLabel->setText("输出信息：");
    ui->statusbar->addWidget(statusLabel);
    output = new myLabel(this);
    ui->statusbar->addWidget(output);
    val = new myLabel(this);
    ui->statusbar->addWidget(val);

    //设置信号槽连接
    //打开
    connect(ui->actionOpenFile, &QAction::triggered, this, &MainWindow::openFile);
    //退出
    connect(ui->actionExit, &QAction::triggered,this,[=](){
        this->close();
    });

    connect(ui->actionFlipUD,&QAction::triggered,this,&MainWindow::flipUD);

    connect(ui->actionFlipRL,&QAction::triggered,this,&MainWindow::flipRL);

    connect(ui->actionSmall,&QAction::triggered,this,&MainWindow::resizeImg);

    connect(ui->actionCut,&QAction::triggered,this,&MainWindow::openDialog);

    connect(ui->label,&myLabel::clicked,this,&MainWindow:: showInfo);

    connect(ui->action90,&QAction::triggered,this,&MainWindow::rotato);



    connect(ui->actionMean,&QAction::triggered,this,&MainWindow::showMean);

    //滤波
    connect(ui->actionMeanFilter,&QAction::triggered,this,&MainWindow::meanFilter);

    connect(ui->actionMedianFilter,&QAction::triggered,this,&MainWindow::medianFilter);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("选择图像"), "D:/image/", tr("Images(*.bmp)"));

    if (fileName.isEmpty())
    {
        QMessageBox::critical(this,"提示","文件路径无效",QMessageBox::Ok);
        return;
    }
    else
    {
        qDebug()<<fileName;
        myImg = new Image(fileName.toUtf8().data());
        myImg->Flip(1);
        int linebyte = (myImg->getWidth() * 8 + 31) / 32 * 4;

        QImage* tempImage = new QImage(myImg->getImgPointer(), myImg->getWidth(), myImg->getHeight(),linebyte, QImage::Format_Grayscale8);
        //QImage tempImage(fileName);
        ui->label->setAlignment(Qt::AlignTop);
        ui->label->setPixmap(QPixmap::fromImage(*tempImage));
    }
}

//上下翻转
void MainWindow::flipUD()
{
    myImg->Flip(1);
    int linebyte = (myImg->getWidth() * 8 + 31) / 32 * 4;
    QImage* tempImage = new QImage(myImg->getImgPointer(), myImg->getWidth(), myImg->getHeight(),linebyte, QImage::Format_Grayscale8);
     ui->label->setAlignment(Qt::AlignTop);
    ui->label->setPixmap(QPixmap::fromImage(*tempImage));
}
//左右翻转
void MainWindow::flipRL()
{
    myImg->Flip(0);
    int linebyte = (myImg->getWidth() * 8 + 31) / 32 * 4;
    QImage* tempImage = new QImage(myImg->getImgPointer(), myImg->getWidth(), myImg->getHeight(),linebyte, QImage::Format_Grayscale8);
     ui->label->setAlignment(Qt::AlignTop);
    ui->label->setPixmap(QPixmap::fromImage(*tempImage));
}

//改变大小
void MainWindow::resizeImg()
{
    myImg->Resize(0);
    int linebyte = (myImg->getWidth() * 8 + 31) / 32 * 4;
    QImage* tempImage = new QImage(myImg->getImgPointer(), myImg->getWidth(), myImg->getHeight(),linebyte, QImage::Format_Grayscale8);
    ui->label->setAlignment(Qt::AlignTop);
    ui->label->setPixmap(QPixmap::fromImage(*tempImage));
}

void MainWindow::showInfo(int x, int y)
{
    QString xy ;

    if(y>myImg->getHeight()||x>myImg->getWidth())
    {
        output->setText("坐标:(-,-)");
        val->setText("-");
    }
    else
    {
        xy = QString("坐标:(%1, %2)").arg(x).arg(y);
        output->setText(xy);
        val->setText(QString::number(int(myImg->At(x,y))));
    }
}

void MainWindow::openDialog()
{
    //设置打开对话框
   Dialog *dlg = new Dialog(this);
   dlg->show();
   //获取裁剪区域数据
   connect(dlg,&Dialog::sendAera,this,&MainWindow::getCutAera);

}

void MainWindow::getCutAera(int x1,int y1, int x2,int y2)
{
    myImg->Cut(x1,y1,x2,y2);
    int linebyte = (myImg->getWidth() * 8 + 31) / 32 * 4;
    QImage* tempImage = new QImage(myImg->getImgPointer(), myImg->getWidth(), myImg->getHeight(),linebyte, QImage::Format_Grayscale8);
    ui->label->setAlignment(Qt::AlignTop);
    ui->label->setPixmap(QPixmap::fromImage(*tempImage));
}

void MainWindow::rotato()
{
    myImg->Rotate(90);
    int linebyte = (myImg->getHeight() * 8 + 31) / 32 * 4;
    QImage* tempImage = new QImage(myImg->getImgPointer(), myImg->getHeight(), myImg->getWidth(),linebyte, QImage::Format_Grayscale8);
    ui->label->setAlignment(Qt::AlignTop);
    ui->label->setPixmap(QPixmap::fromImage(*tempImage));
}

void MainWindow::showMean()
{
    float m,var;
    myImg->Mean_Variance(m,var);
    QString text = QString("平均值：%1 \n 方差：%2").arg(m).arg(var);
    QMessageBox::information(this,"统计信息",text,QMessageBox::Ok);
}

void MainWindow::meanFilter()
{
    //Image img("Lena_gaussian.bmp");

    int h=myImg->getHeight();
    int w=myImg->getWidth();

    Filter *filter = NULL;
    int size = QInputDialog::getInt(this,
                                        "等待用户输入",
                                        "请输入FilterSize",
                                        3,				//默认值
                                        0,				//最小值
                                        50,			//最大值
                                        1);				//步进);


    filter = new MeanFilter(size);
    const Matrix input(myImg->getHeight(),myImg->getWidth());
    for(int i=0;i<myImg->getHeight();i++)
    {
        for(int j=0;j<myImg->getWidth();j++)
        {
            input.Set(i,j,myImg->At(i,j));
        }
    }
   Image result_mean(h,w);

    result_mean = filter->Filtering(input);//右边返回一个Matrix类型的引用
    qDebug()<<"在mainwindow类中，filter函数执行结束，下面开始显示图像";
    // 显示该结果图像
    int linebyte = (result_mean.getWidth() * 8 + 31) / 32 * 4;
    QImage* tempImage = new QImage(result_mean.getImgPointer(), result_mean.getWidth(), result_mean.getHeight(),linebyte, QImage::Format_Grayscale8);
    ui->label->setAlignment(Qt::AlignTop);
    ui->label->setPixmap(QPixmap::fromImage(*tempImage));

    delete filter;
}

void MainWindow::medianFilter()
{
    int h=myImg->getHeight();
    int w=myImg->getWidth();
    Filter *filter = NULL;
    int size = QInputDialog::getInt(this,
                                        "等待用户输入",
                                        "请输入FilterSize",
                                        3,				//默认值
                                        0,				//最小值
                                        50,			//最大值
                                        1);				//步进);

    filter = new MedianFilter(size);//事实上没用的这个size，固定为3

    const Matrix input(myImg->getHeight(),myImg->getWidth());
    for(int i=0;i<myImg->getHeight();i++)
    {
        for(int j=0;j<myImg->getWidth();j++)
        {
            input.Set(i,j,myImg->At(i,j));
        }
    }
    Image result_median(h,w);
    result_median = filter->Filtering(input);

    int linebyte = (result_median.getWidth() * 8 + 31) / 32 * 4;
    QImage* tempImage = new QImage(result_median.getImgPointer(), result_median.getWidth(), result_median.getHeight(),linebyte, QImage::Format_Grayscale8);
    ui->label->setAlignment(Qt::AlignTop);
    ui->label->setPixmap(QPixmap::fromImage(*tempImage));
   // 显示该结果图像
   delete filter;

}

