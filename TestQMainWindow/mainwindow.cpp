#include "mainwindow.h"

#include "versiondialog.h"

#include <QColorDialog>
#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QStatusBar>
#include <QTextEdit>
#include <QToolBar>
#include <QtPrintSupport/QPageSetupDialog>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrintPreviewDialog>
#include <QtPrintSupport/QPrinter>
#include <QtPrintSupport/QPrinterInfo>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("主窗口");
    setWindowIcon(QIcon(":/Image/up.png"));
    resize(1000, 800);
    //创建一个菜单栏
    QMenuBar *mBar = menuBar();
    //将menu bar放入到主窗口
    setMenuBar(mBar);

    //创建菜单
    QMenu *fileMenu   = mBar->addMenu("文件");
    QMenu *editMenu   = mBar->addMenu("编辑");
    QMenu *dialogMenu = mBar->addMenu("对话框");
    QMenu *helpMenu   = mBar->addMenu("帮助");

    //添加菜单项
    QAction *openAction  = fileMenu->addAction("打开");
    QAction *closeAction = fileMenu->addAction("关闭");
    fileMenu->addSeparator();
    QAction *saveAction = fileMenu->addAction("保存");

    QAction *copyAction  = editMenu->addAction("复制");
    QAction *parseAction = editMenu->addAction("粘贴");

    QAction *modalAction    = dialogMenu->addAction("模态");
    QAction *modelessAction = dialogMenu->addAction("非模态");
    QAction *aboutAction    = dialogMenu->addAction("关于");
    QAction *colorAction    = dialogMenu->addAction("颜色");

    QAction *versionAction = helpMenu->addAction("版本号");

    // QString pathname;
    connect(openAction, &QAction::triggered, this, [=]() { QString pathname = QFileDialog::getOpenFileName(this, "文件浏览", "D:\\softwareData", "Image Files(*.jpg *,png);;Text(*.txt)"); });

    connect(modalAction, &QAction::triggered, this, [=]() {
        QDialog dialog(this);
        dialog.setWindowTitle("模态对话框");
        dialog.resize(400, 300);
        dialog.exec();

        QPushButton *abtn = new QPushButton("按钮", &dialog);
        // abtn->move(100,100);
    });

    connect(modelessAction, &QAction::triggered, this, [=]() {
        QDialog *dialog = new QDialog(this);
        dialog->setWindowTitle("非模态对话框");
        dialog->resize(300, 200);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->show();
    });

    connect(aboutAction, &QAction::triggered, this, [=]() { QMessageBox::critical(this, "关键的", "错误"); });

    connect(colorAction, &QAction::triggered, this, [=]() {
        QColor color = QColorDialog::getColor(QColor(255, 0, 0));
        qDebug() << color.red();
        qDebug() << color.green();
        qDebug() << color.blue();
    });

    m_versionDialog = new VersionDialog(this);
    connect(versionAction, &QAction::triggered, this, [=]() {
        m_versionDialog->setWindowTitle("版本信息");
        m_versionDialog->show();
    });
    openAction->setIcon(QIcon(":/Image/Luffy.png"));
    //创建工具栏
    QToolBar *toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::LeftToolBarArea);
    toolBar->addAction(openAction);
    toolBar->addAction(closeAction);
    toolBar->addSeparator();
    toolBar->addAction(copyAction);
    toolBar->addAction(parseAction);

    //中心部件
    QTextEdit *edit = new QTextEdit(this);
    setCentralWidget(edit);

    QPushButton *newBtn = new QPushButton("打印", this);
    toolBar->addWidget(newBtn);

    connect(newBtn, &QPushButton::clicked, this, [=]() {
        //获取打印机列表
        QPrinterInfo info;

        QString name = info.defaultPrinterName(); // 默认打印机名字
        qDebug() << name;
        QStringList list = info.availablePrinterNames(); // 各个打印机名字
        qDebug() << list;
        QPrinter::DuplexMode mode = info.defaultDuplexMode(); // 返回此打印机的默认双面打印模式。

        enum DuplexMode
        {
            DuplexNone = 0,
            DuplexAuto,
            DuplexLongSide,
            DuplexShortSide
        };

        //        QPrinter printer;

        QPrinter printer;
        QPrintDialog printDialog(&printer, this);
        if (printDialog.exec()) { edit->print(&printer); }

        //        QPageSetupDialog* setup = new QPageSetupDialog(&printer, this);
        //        if (setup->exec() == QDialog::Accepted) {
        //            printer.setOrientation(QPrinter::Landscape);
        //        } else {
        //            printer.setOrientation(QPrinter::Portrait);
        //        }
        QPrintPreviewDialog preview(&printer, this);                                                               // 创建打印预览对话框
        connect(&preview, &QPrintPreviewDialog::paintRequested, [=](QPrinter *printer) { edit->print(printer); }); // 当要生成预览页面时，发射paintRequested()信号
        preview.exec();
    });

    //创建状态栏
    QStatusBar *stBar = statusBar();
    setStatusBar(stBar);
    QLabel *label1 = new QLabel("状态", this);
    stBar->addWidget(label1);
    QLabel *label2 = new QLabel("byte", this);
    stBar->addPermanentWidget(label2);

    //铆接部件,如果没有中心部件，会占据中心位置
    QDockWidget *dockWidget = new QDockWidget("浮动", this);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);
}

MainWindow::~MainWindow()
{
}

void MainWindow::myMenuBar()
{
}
