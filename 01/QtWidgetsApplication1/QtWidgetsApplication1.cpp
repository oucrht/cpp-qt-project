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
    //�������г���ʱ���ú����ᱻ�Զ����ã�ǰ���ػ溯�������Ѿ�˵����Ŷ��
   //�������ǵ�һ����Ҫ�л��ʣ����ǽ���һ�����ʶ���
    QPainter painter(this);
    

    //���û�����ɫ�Ϳ��
    painter.setBrush(Qt::black);
    painter.drawRect(0, 0, 500, 500);
    painter.setPen(QColor(255,255,255));
    painter.drawEllipse(0, 0, 500, 500);//��Բ
    
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