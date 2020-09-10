#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = nullptr);
    ~Dialog();

    void activeSig();
signals:
    void sendAera(int x1,int y1,int x2,int y2);
private:
    Ui::Dialog *ui;
    int x1;
    int x2;
    int y1;
    int y2;
};

#endif // DIALOG_H
