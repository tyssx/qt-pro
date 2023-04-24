//#include "vxlib.h"
//#include "vxlogger.h"
#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();
    //初始化
    vxInitInstance();
    vxInit();
    return a.exec();
}
