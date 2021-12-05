#include "mainwindow.h"

#include "colorsetdialog.h"
#include "listwidgets.h"
#include "tablewidgets.h"
#include "timedatesetdialog.h"
#include "treewidgets.h"
#include "versiondialog.h"
#include "wordwidget.h"

#include <QColorDialog>
#include <QCoreApplication>
#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QFontComboBox>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QLabel>
#include <QMenuBar>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSpinBox>
#include <QStatusBar>
#include <QTextBlock>
#include <QTextCodec>
#include <QTextEdit>
#include <QTimer>
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
    setWindowTitle("主窗口[*]");
    setWindowIcon(QIcon(":/Image/up.png"));
    setToolButtonStyle(Qt::ToolButtonTextUnderIcon); //设置工具栏风格
    resize(1400, 900);

    //中心部件
    textEdit = new QTextEdit(this);
    setCentralWidget(textEdit);

    //创建一个菜单栏
    QMenuBar *mBar = menuBar();
    //将menu bar放入到主窗口
    setMenuBar(mBar);

    //创建菜单
    QMenu *fileMenu = mBar->addMenu("文件");
    QMenu *editMenu = mBar->addMenu("编辑");
    QMenu *setMenu  = mBar->addMenu("设置");
    QMenu *itemMenu = mBar->addMenu("Item");
    QMenu *helpMenu = mBar->addMenu("帮助");

    //添加菜单项
    QAction *openAction = fileMenu->addAction(QIcon(":/Image/open1.bmp"), "打开");

    QAction *closeAction = fileMenu->addAction(QIcon(":/Image/close1.bmp"), "关闭");
    fileMenu->addSeparator();
    QAction *saveAction   = fileMenu->addAction(QIcon(":/Image/save1.bmp"), "保存");
    QAction *saveAsAction = fileMenu->addAction("另存为");

    QAction *copyAction  = editMenu->addAction(QIcon(":/Image/copy.bmp"), "复制");
    QAction *cutAction   = editMenu->addAction(QIcon(":/Image/cut.bmp"), "剪贴");
    QAction *pasteAction = editMenu->addAction(QIcon(":/Image/paste.bmp"), "粘贴");

    QAction *colorSelectAction = setMenu->addAction("颜色选择");
    QAction *wordSetAction     = setMenu->addAction("文字设置");
    QAction *colorSetAction    = setMenu->addAction("颜色设置");
    QAction *datatimeSetAction = setMenu->addAction("时间日期设置");

    QAction *listWgsAction  = itemMenu->addAction("List Widgets");
    QAction *treeWgsAction  = itemMenu->addAction("Tree Widgets");
    QAction *tableWgsAction = itemMenu->addAction("Table Widgets");

    QAction *aboutAction   = helpMenu->addAction("关于");
    QAction *versionAction = helpMenu->addAction("版本号");

    // QString pathname;
    connect(openAction, &QAction::triggered, this, &MainWindow::slot_openFile);
    connect(closeAction, &QAction::triggered, this, &MainWindow::slot_closeFile);
    connect(saveAction, &QAction::triggered, this, &MainWindow::slot_saveFile);

    connect(copyAction, &QAction::triggered, this, [=]() { textEdit->copy(); });
    connect(cutAction, &QAction::triggered, this, [=]() { textEdit->cut(); });
    connect(pasteAction, &QAction::triggered, this, [=]() { textEdit->paste(); });

    //拷贝有效
    connect(textEdit, &QTextEdit::copyAvailable, this, [=](bool avail) {
        copyAction->setEnabled(avail);
        cutAction->setEnabled(avail);
        pasteAction->setEnabled(textEdit->canPaste());
    });

    //文档被修改时，设置标题设置*，但必须首先在setWindowTitle末尾加"[*]"
    connect(textEdit->document(), &QTextDocument::contentsChanged, [=]() { this->setWindowModified(true); });

    connect(aboutAction, &QAction::triggered, this, [=]() { QMessageBox::critical(this, "关键的", "错误"); });

    connect(colorSelectAction, &QAction::triggered, this, [=]() {
        QColor color = QColorDialog::getColor(QColor(255, 0, 0));
        qDebug() << color.red();
        qDebug() << color.green();
        qDebug() << color.blue();
    });

    //全局，创建一直存在
    m_wordWidget = new WordWidget();
    m_wordWidget->setVisible(false);
    connect(wordSetAction, &QAction::triggered, this, [=]() {
        m_wordWidget->setWindowTitle("文字设置");
        m_wordWidget->show();
        m_wordWidget->raise();
    });

    connect(colorSetAction, &QAction::triggered, this, [=]() {
        //模态对话框 局部，重新打开重新生成
        m_colorSetDialog = new ColorSetDialog(this);
        m_colorSetDialog->setWindowTitle("颜色设置");
        m_colorSetDialog->exec();
    });

    connect(datatimeSetAction, &QAction::triggered, this, [=]() {
        //非模态对话框
        m_timeDateSetDialog = new TimeDateSetDialog(this);
        m_timeDateSetDialog->setWindowTitle("日期时间设置");
        m_timeDateSetDialog->show();
    });

    //    connect(listWgsAction, &QAction::triggered, this, [=]() {
    //        m_listWidgets = new ListWidgets(this);
    //        m_listWidgets->show();
    //        m_listWidgets->raise();
    //    });
    connect(mBar, &QMenuBar::triggered, this, [=](QAction *action) {
        QMainWindow *mainWindow;
        if (action == listWgsAction)
        {
            //            m_listWidgets = new ListWidgets(this);
            //            m_listWidgets->show();
            //            m_listWidgets->raise();
            mainWindow = new ListWidgets(this);
        }
        else if (action == treeWgsAction)
        {
            //            m_treeWidgets = new TreeWidgets(this);
            //            m_treeWidgets->show();
            //            m_treeWidgets->raise();
            mainWindow = new TreeWidgets(this);
        }
        else if (action == tableWgsAction)
        {
            mainWindow = new TableWidgets(this);
        }
        mainWindow->show();
        mainWindow->raise();
    });

    //全局，创建一直存在
    m_versionDialog = new VersionDialog(this);
    connect(versionAction, &QAction::triggered, this, [=]() {
        m_versionDialog->setWindowTitle("版本信息");
        m_versionDialog->show();
    });

    //创建工具栏
    QToolBar *toolBar = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, toolBar);

    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::LeftToolBarArea);
    toolBar->addAction(openAction);
    toolBar->addAction(closeAction);
    toolBar->addAction(saveAction);
    toolBar->addSeparator();
    toolBar->addAction(copyAction);
    toolBar->addAction(cutAction);
    toolBar->addAction(pasteAction);

    toolBar->addSeparator();
    QAction *actionFontBold = new QAction("粗体", this);
    actionFontBold->setCheckable(true);
    actionFontBold->setChecked(false);
    connect(actionFontBold, &QAction::triggered, this, [=](bool checked) {
        QTextCharFormat fmt;
        fmt = textEdit->currentCharFormat();
        if (checked) { fmt.setFontWeight(QFont::Bold); }
        else
        {
            fmt.setFontWeight(QFont::Normal);
        }
        textEdit->mergeCurrentCharFormat(fmt);
    });
    toolBar->addAction(actionFontBold);
    connect(textEdit, &QTextEdit::selectionChanged, this, [=]() {
        QTextCharFormat fmt;
        fmt = textEdit->currentCharFormat();
        actionFontBold->setChecked(fmt.font().bold()); //是否粗体
    });

    m_spinFontSize = new QSpinBox;
    m_spinFontSize->setMaximumWidth(50);
    m_spinFontSize->setMinimum(5);
    m_spinFontSize->setMaximum(50);
    m_spinFontSize->setValue(textEdit->font().pointSize());
    toolBar->addWidget(new QLabel("字体大小 "));
    toolBar->addWidget(m_spinFontSize);

    void (QSpinBox::*valueChangeSignal)(int) = &QSpinBox::valueChanged;
    connect(m_spinFontSize, valueChangeSignal, this, [=](int size) {
        QTextCharFormat fmt;
        fmt.setFontPointSize(size);
        textEdit->mergeCurrentCharFormat(fmt);
    });

    toolBar->addWidget(new QLabel(" 字体 "));
    m_fontComboBox = new QFontComboBox;
    m_fontComboBox->setMinimumWidth(150);
    m_fontComboBox->setMaximumWidth(150);
    toolBar->addWidget(m_fontComboBox);
    void (QFontComboBox::*currentIndexChangedSignal)(const QString &) = &QFontComboBox::currentIndexChanged;
    connect(m_fontComboBox, currentIndexChangedSignal, this, [=](const QString &fontName) {
        QTextCharFormat fmt;
        fmt.setFontFamily(fontName);
        textEdit->mergeCurrentCharFormat(fmt);
    });

    toolBar->addSeparator();

    m_printer              = new QPrinter; //创建打印机对象
    QAction *actionPreview = new QAction(QIcon(":/Image/preview.bmp"), "打印预览", this);
    toolBar->addAction(actionPreview);
    connect(actionPreview, &QAction::triggered, this, &MainWindow::slot_printPreview);

    QPushButton *printBtn = new QPushButton(QIcon(":/Image/print.bmp"), "打印", this);
    toolBar->addWidget(printBtn);
    connect(printBtn, &QPushButton::clicked, this, &MainWindow::slot_printDocument);

    //创建状态栏
    QStatusBar *stBar = statusBar();
    setStatusBar(stBar);
    labelFile = new QLabel("当前文件：", this);
    labelFile->setMinimumWidth(300); //设置最小宽度
    labelFile->setMaximumWidth(300);
    stBar->addWidget(labelFile);

    QLabel *label1 = new QLabel("状态：", this);
    stBar->addWidget(label1);

    m_progress = new QProgressBar(this);
    m_progress->setMaximumWidth(200); //设置最大宽度
    m_progress->setMaximum(100);
    m_progress->setMinimum(1);
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [&]() {
        a++;
        if (a > 100) a = 0;
        m_progress->setValue(a);
    });
    timer->start(1000);

    stBar->addWidget(m_progress);
    labelByte = new QLabel(" byte", this);
    stBar->addPermanentWidget(labelByte);

    //铆接部件,如果没有中心部件，会占据中心位置
    dockWidget = new QDockWidget("浮动", this);
    dockWidget->setMinimumWidth(300);
    plainText = new QPlainTextEdit(dockWidget);
    dockWidget->setWidget(plainText);
    addDockWidget(Qt::RightDockWidgetArea, dockWidget);

    installEventFilter(this); //安装事件过滤器
}

MainWindow::~MainWindow()
{
    qDebug() << "~MainWindow";
    delete m_wordWidget;
}

void MainWindow::myMenuBar()
{
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    //要使用过滤器，首先要安装过滤器事件在所需目标上，如installEventFilter(this);安装在整个窗口
    do
    {
        if (event == nullptr) { break; }
        if (event->type() == QEvent::KeyPress) //按键按下
        {
            QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
            if ((keyEvent->key() == Qt::Key_S) && (keyEvent->modifiers() == Qt::ControlModifier))
            {
                // Ctrl+S
                slot_saveFile();
            }
        }
    } while (0);
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::dockShowContents(QString contents)
{
    qDebug() << "dockShowContents";
    plainText->setPlainText(contents);
    QTextDocument *doc = plainText->document(); //文本对象
    int cnt            = doc->blockCount();     //获取文本块个数，回车符是一个block
    for (int c = 0; c < 6; c++) plainText->appendPlainText("");

    QPalette paltte;
    paltte = plainText->palette();
    paltte.setColor(QPalette::Text, Qt::red);
    plainText->setPalette(paltte);
    plainText->appendPlainText(QString::asprintf("共有文本块%d个", cnt));
    for (int i = 0; i < cnt; i++)
    {
        QTextBlock textLine = doc->findBlockByNumber(i); //获取文本的一段
        QString str         = textLine.text();
        plainText->appendPlainText(str);
    }
}

void MainWindow::slot_openFile()
{
    QString disPlayStr;
    curPathname = nullptr;
    curPathname = QFileDialog::getOpenFileName(this, "文件浏览", QCoreApplication::applicationDirPath(), "Text(*.txt);;Image Files(*.jpg *,png)");
    if (curPathname.isNull())
    {
        QMessageBox::warning(this, tr("温馨提示"), tr("打开文件失败！"));
        return;
    }
    QFileInfo fileInfo(curPathname); //文件信息
    QFile file(curPathname);         //文件
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "警告", "打开文件失败！");
        return;
    }

    labelFile->setToolTip(curPathname);
    labelFile->setText(QString("当前文件：%1").arg(fileInfo.fileName())); //文件名
    setWindowTitle(fileInfo.fileName() + "[*]");

    while (!file.atEnd())
    {
        QByteArray line = file.readLine();
        QString str(line);
        disPlayStr += str;
    }
    file.close();
    textEdit->clear();
    textEdit->setPlainText(disPlayStr);
    labelByte->setText(QString::asprintf("%d byte", disPlayStr.size()));
    setWindowModified(false); //去掉*
    dockShowContents(disPlayStr);
}

void MainWindow::slot_closeFile()
{
    textEdit->clear();
    curPathname = nullptr;
    labelFile->setText("当前文件：");
    labelByte->setText(" byte");
    setWindowTitle("主窗口[*]");
    setWindowModified(false);
}

void MainWindow::slot_saveFile()
{
    if (!isWindowModified()) return; //没有*说明已保存，直接退出
    QFile file;
    if (!curPathname.isNull())
    {
        file.setFileName(curPathname); //文件
    }
    else
    {
        QString savePath = QFileDialog::getSaveFileName(this, "文件保存", QCoreApplication::applicationDirPath(), "Text(*.txt);;Image Files(*.jpg *,png)");
        if (savePath.isNull())
        {
            QMessageBox::critical(this, "警告", "文件保存失败！");
            return;
        }
        file.setFileName(savePath);   //文件
        QFileInfo fileInfo(savePath); //文件信息
        setWindowTitle(fileInfo.fileName() + "[*]");
        labelFile->setToolTip(savePath);
        labelFile->setText(QString("当前文件：%1").arg(fileInfo.fileName())); //文件名
        curPathname = savePath;
    }
    if (!file.open(QIODevice::WriteOnly | QFile::Text))
    {
        QMessageBox::critical(this, "关键的", "打开文件失败！");
        return;
    }

    QByteArray strBytes = textEdit->toPlainText().toUtf8();
    file.write(strBytes);
    labelByte->setText(QString::asprintf("%d byte", strBytes.size()));
    file.close();
    this->setWindowModified(false); //*去除
}

void MainWindow::slot_printDocument()
{
    //获取打印机列表
    QPrinterInfo info;

    QString name = info.defaultPrinterName(); // 默认打印机名字
    qDebug() << name;
    QStringList list = info.availablePrinterNames(); // 各个打印机名字
    qDebug() << list;
    QPrinter::DuplexMode mode = info.defaultDuplexMode(); // 返回此打印机的默认双面打印模式。

    QPrintDialog printDialog(m_printer, this);
    if (textEdit->textCursor().hasSelection()) printDialog.addEnabledOption(QAbstractPrintDialog::PrintSelection);
    if (printDialog.exec() == QDialog::Accepted) { textEdit->print(m_printer); }

    //        QPageSetupDialog* setup = new QPageSetupDialog(&printer, this);
    //        if (setup->exec() == QDialog::Accepted) {
    //            printer.setOrientation(QPrinter::Landscape);
    //        } else {
    //            printer.setOrientation(QPrinter::Portrait);
    //        }
}

void MainWindow::slot_printPreview()
{
    QPrintPreviewDialog preview(m_printer, this);                                                                  // 创建打印预览对话框
    connect(&preview, &QPrintPreviewDialog::paintRequested, [=](QPrinter *printer) { textEdit->print(printer); }); // 当要生成预览页面时，发射paintRequested()信号
    preview.exec();
}
