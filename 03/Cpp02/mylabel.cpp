#include "mylabel.h"

myLabel::myLabel(QWidget *parent) : QLabel(parent)
{

}

void myLabel::mousePressEvent(QMouseEvent *ev)
{
//   x=ev->x();
//   y=ev->y();

   emit clicked(ev->x(),ev->y());

}





int myLabel::getX()
{
    return x;
}

int myLabel::getY()
{
    return y;
}
