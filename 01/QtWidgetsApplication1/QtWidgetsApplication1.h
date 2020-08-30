#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QtWidgetsApplication1.h"

class QtWidgetsApplication1 : public QMainWindow
{
    Q_OBJECT

public:
    QtWidgetsApplication1(QWidget *parent = Q_NULLPTR);
    ~QtWidgetsApplication1();
    void paintEvent(QPaintEvent*);
        
protected:
    void timerEvent(QTimerEvent* event);
    int m_nTimerId;
private:
    Ui::QtWidgetsApplication1Class ui;
};

