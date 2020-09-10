#ifndef MYLABEL_H
#define MYLABEL_H

#include <QWidget>
#include <QLabel>
#include <QDebug>
#include <QMouseEvent>

class myLabel : public QLabel
{
    Q_OBJECT
public:
    explicit myLabel(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *ev) override;
//    void mouseMoveEvent(QMouseEvent *ev) override;
//    void mouseReleaseEvent(QMouseEvent *ev) override;
    int getX();
    int getY();
signals:
    void clicked(int x,int y);

private:
    int x;
    int y;
};

#endif // MYLABEL_H
