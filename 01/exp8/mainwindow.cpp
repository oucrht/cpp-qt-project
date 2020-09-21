#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QDateTime>
#include <math.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    x = new double[800];
    y = new double[800];
    QTimer *timer = new QTimer(this);
    timer->start(500);

    connect(timer,&QTimer::timeout,this,[=](){
        int i = 0,k=0;
        update();
        for (i = 0; i < 800; i++)
        {
            Q = sqrt((x[i] - 300.0) * (x[i] - 300.0) + (y[i] - 300.0) * (y[i] - 300.0));

            if (Q < 300)
            {
                k++;
            }
        }
        numIn += k;
        pi2 = (4.0 * numIn) / (800.0 * A);
        z = pi1 - pi2;
        m = fabs(z);
        pi1 = pi2;
        A++;
        ui->label->setText(QString::number(pi1,'f',8));




    });


}

MainWindow::~MainWindow()
{
    delete ui;
    delete x;
    delete y;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, 600, 600);
    painter.setPen(QColor(255,255,255));
    painter.drawEllipse(0, 0, 600, 600);//画圆
    QDateTime time = QDateTime::currentDateTime();
    int i = 0;

     srand((unsigned int)time_t(time.toTime_t()));

     for (i=0; i < 800; i++)
     {
         x[i] = 1.0 * (rand() % RAND_MAX) / (double)RAND_MAX * 600;

         y[i] = 1.0 * (rand() % RAND_MAX) / (double)RAND_MAX * 600;
         painter.drawPoint(QPoint(x[i], y[i]));
     }
}


