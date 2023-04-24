#include "mainwindow.h"
#include <QApplication>
#include <QPixmap>
#include <QSplashScreen>
#include <QElapsedTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(":/Image/shij.jpg");
    QSplashScreen screen(pixmap);
    QLabel label(&screen);
    screen.show();

    int delayTime = 1;
    QElapsedTimer timer;
    timer.start();
    while(timer.elapsed() < (delayTime * 1000))
    {
        a.processEvents();
    }

    MainWindow w;
    w.show();

    screen.finish(&w);
    return a.exec();
}
