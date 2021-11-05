#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class VersionDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void myMenuBar();
    void myToolBar();
    VersionDialog *m_versionDialog;
};
#endif // MAINWINDOW_H
