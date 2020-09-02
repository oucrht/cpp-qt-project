#include "QtApp2.h"
#include <QtWidgets/QApplication>
#include <qtextcodec.h>

int main(int argc, char *argv[])
{
    
    QApplication a(argc, argv);
    QtApp2 w;
    w.show();
    return a.exec();
}
