#include "QtWidgetsApplication1.h"
#include <qpainter.h>
QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void QtWidgetsApplication1::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::red, 10));
    painter.drawPoint(200, 200);
    painter.drawText(QPoint(10, 70), "hello");
}

