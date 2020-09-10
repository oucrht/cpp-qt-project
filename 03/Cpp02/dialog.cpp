#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(ui->pushButtonOk,&QPushButton::clicked,this,&Dialog::activeSig);
    connect(ui->pushButtonCancel,&QPushButton::clicked,this,&Dialog::close);
    connect(ui->pushButtonOk,&QPushButton::clicked,this,&Dialog::close);
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::activeSig()
{
    QString temp = ui->lineEditx1->text();
    x1=temp.toInt();
    temp=ui->lineEdity1->text();
    y1 = temp.toInt();
    temp=ui->lineEditx2->text();
    x2=temp.toInt();
    temp=ui->lineEdity2->text();
    y2=temp.toInt();
    emit sendAera(x1,y1,x2,y2);
}

