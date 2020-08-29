#include "QtWidgetsApplication1.h"
#include <qpainter.h>
#include <Windows.h>
QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void QtWidgetsApplication1::paintEvent(QPaintEvent*)
{
    //当你运行程序时，该函数会被自动调用，前面重绘函数部分已经说过了哦。
   //现在我们第一步是要有画笔，我们建立一个画笔对象
    QPainter painter(this);
    

    //设置画笔颜色和宽度
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, 500, 500);
    painter.setPen(QColor(255,255,255));
    painter.drawEllipse(0, 0, 500, 500);//画圆
    
    int i = 0;
    int x, y;
    srand((unsigned int)time_t(0));
    for (i; i < 100; i++)
    {
        
        x = 1.0 * (rand() % RAND_MAX) / RAND_MAX * 500;
        y = 1.0 * (rand() % RAND_MAX) / RAND_MAX * 500;
        painter.drawPoint(QPoint(x, y));
        //Sleep(20);
    }
    painter.end();
    update();
}

void QtWidgetsApplication1::timerEvent(QTimerEvent* event)
{
    m_nTimerId = startTimer(100);
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{
    killTimer(m_nTimerId);
}