/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QIcon>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionactionOpenCCF;
    QAction *actionactionSnap;
    QAction *actionactionGrab;
    QAction *actionactionFreeze;
    QWidget *centralWidget;
    QLabel *labelSrc;
    QTextEdit *textEditLog;
    QWidget *widget;
    QGroupBox *groupBox;
    QLineEdit *lineEditXivalue;
    QLabel *label_6;
    QLineEdit *lineEditYivalue;
    QLabel *label_7;
    QLabel *label_8;
    QLabel *label_2;
    QComboBox *comboBoxIP;
    QPushButton *pushBtnConnect;
    QPushButton *pushBtnClose;
    QGroupBox *groupBox_3;
    QPushButton *pushBtnXStop;
    QPushButton *pushBtnXZero;
    QPushButton *pushBtnYZero;
    QLabel *labelYuan;
    QLineEdit *lineEditXimode;
    QLabel *label_9;
    QPushButton *pushBtnClockStart;
    QPushButton *pushBtnAnticlockStart;
    QPushButton *pushBtnRight;
    QPushButton *pushBtnClockwise;
    QPushButton *pushBtnLeft;
    QPushButton *pushBtnOrigin;
    QPushButton *pushBtnAnticlockwise;
    QGroupBox *groupBox_2;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_3;
    QLineEdit *lineEditXPos;
    QLabel *label_4;
    QLineEdit *lineEditYPos;
    QGroupBox *groupBox_6;
    QPushButton *pushBtnAutoSport;
    QPushButton *pushBtnStopAuto;
    QWidget *widget_2;
    QGroupBox *groupBox_4;
    QLabel *label_5;
    QLabel *label_10;
    QLabel *label_11;
    QLineEdit *lineEditTime;
    QSpinBox *spinBlackTolerance;
    QLabel *label_12;
    QSpinBox *spinAreaThreshold;
    QSpinBox *spinWhiteTolerance;
    QGroupBox *groupBox_5;
    QHBoxLayout *horizontalLayout;
    QCheckBox *checkBoxWhite;
    QCheckBox *checkBoxBlack;
    QLabel *labelObj;
    QLabel *labelDealNum;
    QLabel *labelGetNum;
    QPushButton *pushBtnSrcDir;
    QPushButton *pushBtnObjDir;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1191, 751);
        QIcon icon;
        icon.addFile(QString::fromUtf8(":/Image/tub.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        MainWindow->setWindowIcon(icon);
        actionactionOpenCCF = new QAction(MainWindow);
        actionactionOpenCCF->setObjectName(QString::fromUtf8("actionactionOpenCCF"));
        actionactionSnap = new QAction(MainWindow);
        actionactionSnap->setObjectName(QString::fromUtf8("actionactionSnap"));
        actionactionGrab = new QAction(MainWindow);
        actionactionGrab->setObjectName(QString::fromUtf8("actionactionGrab"));
        actionactionFreeze = new QAction(MainWindow);
        actionactionFreeze->setObjectName(QString::fromUtf8("actionactionFreeze"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        labelSrc = new QLabel(centralWidget);
        labelSrc->setObjectName(QString::fromUtf8("labelSrc"));
        labelSrc->setGeometry(QRect(20, 20, 410, 500));
        labelSrc->setFocusPolicy(Qt::NoFocus);
        labelSrc->setContextMenuPolicy(Qt::DefaultContextMenu);
        labelSrc->setFrameShape(QFrame::Box);
        labelSrc->setScaledContents(false);
        textEditLog = new QTextEdit(centralWidget);
        textEditLog->setObjectName(QString::fromUtf8("textEditLog"));
        textEditLog->setGeometry(QRect(20, 580, 831, 121));
        widget = new QWidget(centralWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(860, 10, 311, 481));
        groupBox = new QGroupBox(widget);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(20, 10, 291, 91));
        lineEditXivalue = new QLineEdit(groupBox);
        lineEditXivalue->setObjectName(QString::fromUtf8("lineEditXivalue"));
        lineEditXivalue->setGeometry(QRect(20, 60, 31, 20));
        label_6 = new QLabel(groupBox);
        label_6->setObjectName(QString::fromUtf8("label_6"));
        label_6->setGeometry(QRect(200, 10, 42, 16));
        lineEditYivalue = new QLineEdit(groupBox);
        lineEditYivalue->setObjectName(QString::fromUtf8("lineEditYivalue"));
        lineEditYivalue->setGeometry(QRect(60, 60, 31, 20));
        label_7 = new QLabel(groupBox);
        label_7->setObjectName(QString::fromUtf8("label_7"));
        label_7->setGeometry(QRect(160, 10, 42, 16));
        label_8 = new QLabel(groupBox);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(80, 10, 72, 16));
        label_2 = new QLabel(groupBox);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 30, 16, 25));
        label_2->setMaximumSize(QSize(30, 16777215));
        comboBoxIP = new QComboBox(groupBox);
        comboBoxIP->setObjectName(QString::fromUtf8("comboBoxIP"));
        comboBoxIP->setGeometry(QRect(40, 30, 140, 25));
        comboBoxIP->setMinimumSize(QSize(140, 0));
        pushBtnConnect = new QPushButton(groupBox);
        pushBtnConnect->setObjectName(QString::fromUtf8("pushBtnConnect"));
        pushBtnConnect->setGeometry(QRect(210, 30, 75, 25));
        pushBtnClose = new QPushButton(groupBox);
        pushBtnClose->setObjectName(QString::fromUtf8("pushBtnClose"));
        pushBtnClose->setGeometry(QRect(210, 60, 75, 25));
        groupBox_3 = new QGroupBox(widget);
        groupBox_3->setObjectName(QString::fromUtf8("groupBox_3"));
        groupBox_3->setGeometry(QRect(20, 110, 291, 241));
        pushBtnXStop = new QPushButton(groupBox_3);
        pushBtnXStop->setObjectName(QString::fromUtf8("pushBtnXStop"));
        pushBtnXStop->setGeometry(QRect(220, 30, 63, 23));
        pushBtnXZero = new QPushButton(groupBox_3);
        pushBtnXZero->setObjectName(QString::fromUtf8("pushBtnXZero"));
        pushBtnXZero->setGeometry(QRect(220, 60, 63, 23));
        pushBtnYZero = new QPushButton(groupBox_3);
        pushBtnYZero->setObjectName(QString::fromUtf8("pushBtnYZero"));
        pushBtnYZero->setGeometry(QRect(220, 120, 63, 23));
        labelYuan = new QLabel(groupBox_3);
        labelYuan->setObjectName(QString::fromUtf8("labelYuan"));
        labelYuan->setGeometry(QRect(10, 10, 211, 221));
        labelYuan->setTextFormat(Qt::AutoText);
        labelYuan->setPixmap(QPixmap(QString::fromUtf8(":/Image/yuan.jpg")));
        labelYuan->setAlignment(Qt::AlignCenter);
        lineEditXimode = new QLineEdit(groupBox_3);
        lineEditXimode->setObjectName(QString::fromUtf8("lineEditXimode"));
        lineEditXimode->setGeometry(QRect(20, 190, 63, 20));
        label_9 = new QLabel(groupBox_3);
        label_9->setObjectName(QString::fromUtf8("label_9"));
        label_9->setGeometry(QRect(20, 170, 48, 16));
        pushBtnClockStart = new QPushButton(groupBox_3);
        pushBtnClockStart->setObjectName(QString::fromUtf8("pushBtnClockStart"));
        pushBtnClockStart->setGeometry(QRect(220, 150, 63, 23));
        pushBtnAnticlockStart = new QPushButton(groupBox_3);
        pushBtnAnticlockStart->setObjectName(QString::fromUtf8("pushBtnAnticlockStart"));
        pushBtnAnticlockStart->setGeometry(QRect(220, 180, 63, 23));
        pushBtnRight = new QPushButton(groupBox_3);
        pushBtnRight->setObjectName(QString::fromUtf8("pushBtnRight"));
        pushBtnRight->setGeometry(QRect(30, 100, 50, 50));
        QIcon icon1;
        icon1.addFile(QString::fromUtf8(":/Image/zuo.jpeg"), QSize(), QIcon::Normal, QIcon::Off);
        pushBtnRight->setIcon(icon1);
        pushBtnRight->setIconSize(QSize(50, 50));
        pushBtnRight->setAutoRepeat(false);
        pushBtnClockwise = new QPushButton(groupBox_3);
        pushBtnClockwise->setObjectName(QString::fromUtf8("pushBtnClockwise"));
        pushBtnClockwise->setGeometry(QRect(90, 40, 50, 50));
        QIcon icon2;
        icon2.addFile(QString::fromUtf8(":/Image/shun.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        pushBtnClockwise->setIcon(icon2);
        pushBtnClockwise->setIconSize(QSize(50, 50));
        pushBtnLeft = new QPushButton(groupBox_3);
        pushBtnLeft->setObjectName(QString::fromUtf8("pushBtnLeft"));
        pushBtnLeft->setGeometry(QRect(150, 100, 50, 50));
        QIcon icon3;
        icon3.addFile(QString::fromUtf8(":/Image/you.jpeg"), QSize(), QIcon::Normal, QIcon::Off);
        pushBtnLeft->setIcon(icon3);
        pushBtnLeft->setIconSize(QSize(50, 50));
        pushBtnLeft->setAutoRepeat(false);
        pushBtnOrigin = new QPushButton(groupBox_3);
        pushBtnOrigin->setObjectName(QString::fromUtf8("pushBtnOrigin"));
        pushBtnOrigin->setGeometry(QRect(90, 100, 50, 50));
        QIcon icon4;
        icon4.addFile(QString::fromUtf8(":/Image/0.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        pushBtnOrigin->setIcon(icon4);
        pushBtnOrigin->setIconSize(QSize(50, 50));
        pushBtnAnticlockwise = new QPushButton(groupBox_3);
        pushBtnAnticlockwise->setObjectName(QString::fromUtf8("pushBtnAnticlockwise"));
        pushBtnAnticlockwise->setGeometry(QRect(90, 160, 50, 50));
        QIcon icon5;
        icon5.addFile(QString::fromUtf8(":/Image/ni.jpg"), QSize(), QIcon::Normal, QIcon::Off);
        pushBtnAnticlockwise->setIcon(icon5);
        pushBtnAnticlockwise->setIconSize(QSize(50, 50));
        groupBox_2 = new QGroupBox(widget);
        groupBox_2->setObjectName(QString::fromUtf8("groupBox_2"));
        groupBox_2->setGeometry(QRect(20, 360, 291, 52));
        horizontalLayout_2 = new QHBoxLayout(groupBox_2);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        label_3 = new QLabel(groupBox_2);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        horizontalLayout_2->addWidget(label_3);

        lineEditXPos = new QLineEdit(groupBox_2);
        lineEditXPos->setObjectName(QString::fromUtf8("lineEditXPos"));

        horizontalLayout_2->addWidget(lineEditXPos);

        label_4 = new QLabel(groupBox_2);
        label_4->setObjectName(QString::fromUtf8("label_4"));

        horizontalLayout_2->addWidget(label_4);

        lineEditYPos = new QLineEdit(groupBox_2);
        lineEditYPos->setObjectName(QString::fromUtf8("lineEditYPos"));

        horizontalLayout_2->addWidget(lineEditYPos);

        groupBox_6 = new QGroupBox(widget);
        groupBox_6->setObjectName(QString::fromUtf8("groupBox_6"));
        groupBox_6->setGeometry(QRect(20, 420, 291, 51));
        pushBtnAutoSport = new QPushButton(groupBox_6);
        pushBtnAutoSport->setObjectName(QString::fromUtf8("pushBtnAutoSport"));
        pushBtnAutoSport->setGeometry(QRect(20, 10, 75, 23));
        pushBtnStopAuto = new QPushButton(groupBox_6);
        pushBtnStopAuto->setObjectName(QString::fromUtf8("pushBtnStopAuto"));
        pushBtnStopAuto->setGeometry(QRect(120, 10, 75, 23));
        widget_2 = new QWidget(centralWidget);
        widget_2->setObjectName(QString::fromUtf8("widget_2"));
        widget_2->setGeometry(QRect(860, 500, 311, 211));
        groupBox_4 = new QGroupBox(widget_2);
        groupBox_4->setObjectName(QString::fromUtf8("groupBox_4"));
        groupBox_4->setGeometry(QRect(20, 10, 291, 191));
        label_5 = new QLabel(groupBox_4);
        label_5->setObjectName(QString::fromUtf8("label_5"));
        label_5->setGeometry(QRect(10, 74, 84, 16));
        label_10 = new QLabel(groupBox_4);
        label_10->setObjectName(QString::fromUtf8("label_10"));
        label_10->setGeometry(QRect(10, 100, 48, 16));
        label_11 = new QLabel(groupBox_4);
        label_11->setObjectName(QString::fromUtf8("label_11"));
        label_11->setGeometry(QRect(10, 22, 48, 16));
        lineEditTime = new QLineEdit(groupBox_4);
        lineEditTime->setObjectName(QString::fromUtf8("lineEditTime"));
        lineEditTime->setGeometry(QRect(100, 22, 71, 20));
        spinBlackTolerance = new QSpinBox(groupBox_4);
        spinBlackTolerance->setObjectName(QString::fromUtf8("spinBlackTolerance"));
        spinBlackTolerance->setGeometry(QRect(100, 74, 71, 20));
        spinBlackTolerance->setMaximum(100);
        spinBlackTolerance->setSingleStep(10);
        spinBlackTolerance->setValue(10);
        label_12 = new QLabel(groupBox_4);
        label_12->setObjectName(QString::fromUtf8("label_12"));
        label_12->setGeometry(QRect(10, 48, 84, 16));
        spinAreaThreshold = new QSpinBox(groupBox_4);
        spinAreaThreshold->setObjectName(QString::fromUtf8("spinAreaThreshold"));
        spinAreaThreshold->setGeometry(QRect(100, 100, 71, 20));
        spinAreaThreshold->setMaximum(1000);
        spinAreaThreshold->setSingleStep(10);
        spinAreaThreshold->setValue(20);
        spinWhiteTolerance = new QSpinBox(groupBox_4);
        spinWhiteTolerance->setObjectName(QString::fromUtf8("spinWhiteTolerance"));
        spinWhiteTolerance->setGeometry(QRect(100, 48, 71, 20));
        spinWhiteTolerance->setMaximum(100);
        spinWhiteTolerance->setSingleStep(10);
        spinWhiteTolerance->setValue(10);
        groupBox_5 = new QGroupBox(groupBox_4);
        groupBox_5->setObjectName(QString::fromUtf8("groupBox_5"));
        groupBox_5->setGeometry(QRect(10, 126, 161, 41));
        horizontalLayout = new QHBoxLayout(groupBox_5);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        checkBoxWhite = new QCheckBox(groupBox_5);
        checkBoxWhite->setObjectName(QString::fromUtf8("checkBoxWhite"));
        checkBoxWhite->setChecked(true);

        horizontalLayout->addWidget(checkBoxWhite);

        checkBoxBlack = new QCheckBox(groupBox_5);
        checkBoxBlack->setObjectName(QString::fromUtf8("checkBoxBlack"));

        horizontalLayout->addWidget(checkBoxBlack);

        labelObj = new QLabel(centralWidget);
        labelObj->setObjectName(QString::fromUtf8("labelObj"));
        labelObj->setGeometry(QRect(440, 20, 410, 500));
        labelObj->setFocusPolicy(Qt::NoFocus);
        labelObj->setContextMenuPolicy(Qt::DefaultContextMenu);
        labelObj->setFrameShape(QFrame::Box);
        labelObj->setScaledContents(false);
        labelDealNum = new QLabel(centralWidget);
        labelDealNum->setObjectName(QString::fromUtf8("labelDealNum"));
        labelDealNum->setGeometry(QRect(620, 530, 54, 12));
        labelGetNum = new QLabel(centralWidget);
        labelGetNum->setObjectName(QString::fromUtf8("labelGetNum"));
        labelGetNum->setGeometry(QRect(200, 530, 54, 12));
        pushBtnSrcDir = new QPushButton(centralWidget);
        pushBtnSrcDir->setObjectName(QString::fromUtf8("pushBtnSrcDir"));
        pushBtnSrcDir->setGeometry(QRect(340, 530, 91, 23));
        pushBtnObjDir = new QPushButton(centralWidget);
        pushBtnObjDir->setObjectName(QString::fromUtf8("pushBtnObjDir"));
        pushBtnObjDir->setGeometry(QRect(760, 530, 91, 23));
        MainWindow->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        MainWindow->setStatusBar(statusBar);

        mainToolBar->addAction(actionactionOpenCCF);
        mainToolBar->addAction(actionactionSnap);
        mainToolBar->addAction(actionactionGrab);
        mainToolBar->addAction(actionactionFreeze);
        mainToolBar->addSeparator();

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", nullptr));
        actionactionOpenCCF->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200CCF", nullptr));
#ifndef QT_NO_TOOLTIP
        actionactionOpenCCF->setToolTip(QApplication::translate("MainWindow", "\346\211\223\345\274\200\347\233\270\346\234\272\351\205\215\347\275\256\346\226\207\344\273\266", nullptr));
#endif // QT_NO_TOOLTIP
        actionactionSnap->setText(QApplication::translate("MainWindow", "\345\215\225\345\274\240\351\207\207\351\233\206", nullptr));
#ifndef QT_NO_TOOLTIP
        actionactionSnap->setToolTip(QApplication::translate("MainWindow", "\345\215\225\347\213\254\344\270\200\345\274\240\351\207\207\351\233\206", nullptr));
#endif // QT_NO_TOOLTIP
        actionactionGrab->setText(QApplication::translate("MainWindow", "\350\277\236\347\273\255\351\207\207\351\233\206", nullptr));
        actionactionFreeze->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\351\207\207\351\233\206", nullptr));
        labelSrc->setText(QString());
        groupBox->setTitle(QApplication::translate("MainWindow", "\350\277\236\346\216\245\346\216\247\345\210\266\345\231\250", nullptr));
        lineEditXivalue->setText(QApplication::translate("MainWindow", "2", nullptr));
        label_6->setText(QApplication::translate("MainWindow", "yivalue", nullptr));
        lineEditYivalue->setText(QApplication::translate("MainWindow", "2", nullptr));
        label_7->setText(QApplication::translate("MainWindow", "xivalue", nullptr));
        label_8->setText(QApplication::translate("MainWindow", "\350\204\211\345\206\262\350\276\223\345\207\272\346\250\241\345\274\217", nullptr));
        label_2->setText(QApplication::translate("MainWindow", "IP", nullptr));
        pushBtnConnect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
        pushBtnClose->setText(QApplication::translate("MainWindow", "\346\226\255\345\274\200", nullptr));
        groupBox_3->setTitle(QApplication::translate("MainWindow", "\350\277\220\345\212\250\346\216\247\345\210\266", nullptr));
        pushBtnXStop->setText(QApplication::translate("MainWindow", "\345\267\246\345\217\263\345\201\234\346\255\242", nullptr));
        pushBtnXZero->setText(QApplication::translate("MainWindow", "\345\267\246\345\217\263\347\275\256\351\233\266", nullptr));
        pushBtnYZero->setText(QApplication::translate("MainWindow", "\351\241\272\351\200\206\347\275\256\351\233\266", nullptr));
        labelYuan->setText(QString());
        lineEditXimode->setText(QApplication::translate("MainWindow", "4", nullptr));
        label_9->setText(QApplication::translate("MainWindow", "\345\215\225\350\275\264\345\233\236\351\233\266", nullptr));
#ifndef QT_NO_TOOLTIP
        pushBtnClockStart->setToolTip(QApplication::translate("MainWindow", "\351\241\272\346\227\266\351\222\210\350\277\220\345\212\250\345\274\200\345\247\213/\345\201\234\346\255\242", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnClockStart->setText(QApplication::translate("MainWindow", "\351\241\272\346\227\266\350\275\254\345\212\250", nullptr));
#ifndef QT_NO_TOOLTIP
        pushBtnAnticlockStart->setToolTip(QApplication::translate("MainWindow", "\351\200\206\346\227\266\351\222\210\350\277\220\345\212\250\345\274\200\345\247\213/\345\201\234\346\255\242", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnAnticlockStart->setText(QApplication::translate("MainWindow", "\351\200\206\346\227\266\350\275\254\345\212\250", nullptr));
#ifndef QT_NO_TOOLTIP
        pushBtnRight->setToolTip(QApplication::translate("MainWindow", "\345\220\221\345\267\246\347\247\273\345\212\250", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnRight->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushBtnClockwise->setToolTip(QApplication::translate("MainWindow", " \351\241\272\346\227\266\351\222\210\350\275\254\345\212\250", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnClockwise->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushBtnLeft->setToolTip(QApplication::translate("MainWindow", "\345\220\221\345\217\263\347\247\273\345\212\250", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnLeft->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushBtnOrigin->setToolTip(QApplication::translate("MainWindow", "\346\260\264\345\271\263\345\233\236\345\210\260\345\216\237\347\202\271\344\275\215\347\275\256", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnOrigin->setText(QString());
#ifndef QT_NO_TOOLTIP
        pushBtnAnticlockwise->setToolTip(QApplication::translate("MainWindow", "\351\200\206\346\227\266\351\222\210\350\275\254\345\212\250", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnAnticlockwise->setText(QString());
        groupBox_2->setTitle(QApplication::translate("MainWindow", "\345\256\236\346\227\266\344\275\215\347\275\256", nullptr));
        label_3->setText(QApplication::translate("MainWindow", "\345\267\246\345\217\263", nullptr));
        label_4->setText(QApplication::translate("MainWindow", "\351\241\272\351\200\206", nullptr));
        lineEditYPos->setText(QString());
        groupBox_6->setTitle(QString());
#ifndef QT_NO_TOOLTIP
        pushBtnAutoSport->setToolTip(QApplication::translate("MainWindow", "\350\207\252\345\212\250\345\274\200\345\247\213\346\243\200\346\265\213\350\277\220\345\212\250", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnAutoSport->setText(QApplication::translate("MainWindow", "\350\207\252\345\212\250\346\243\200\346\265\213", nullptr));
#ifndef QT_NO_TOOLTIP
        pushBtnStopAuto->setToolTip(QApplication::translate("MainWindow", "\345\201\234\346\255\242\345\275\223\345\211\215\346\243\200\346\265\213\350\277\220\345\212\250", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnStopAuto->setText(QApplication::translate("MainWindow", "\345\201\234\346\255\242\350\277\220\345\212\250", nullptr));
        groupBox_4->setTitle(QApplication::translate("MainWindow", "\345\233\276\345\203\217\346\243\200\346\265\213", nullptr));
        label_5->setText(QApplication::translate("MainWindow", "\351\273\221\350\211\262\347\274\272\351\231\267\344\272\256\345\272\246\345\267\256", nullptr));
        label_10->setText(QApplication::translate("MainWindow", "\351\235\242\347\247\257\351\230\210\345\200\274", nullptr));
        label_11->setText(QApplication::translate("MainWindow", "\346\243\200\346\265\213\346\227\266\351\225\277", nullptr));
        lineEditTime->setText(QApplication::translate("MainWindow", "0ms", nullptr));
        label_12->setText(QApplication::translate("MainWindow", "\347\231\275\350\211\262\347\274\272\351\231\267\344\272\256\345\272\246\345\267\256", nullptr));
        groupBox_5->setTitle(QString());
        checkBoxWhite->setText(QApplication::translate("MainWindow", "\346\265\213\347\231\275", nullptr));
        checkBoxBlack->setText(QApplication::translate("MainWindow", "\346\265\213\351\273\221", nullptr));
        labelObj->setText(QString());
        labelDealNum->setText(QApplication::translate("MainWindow", "\347\254\254 0 \345\274\240", nullptr));
        labelGetNum->setText(QApplication::translate("MainWindow", "\347\254\254 0 \345\274\240", nullptr));
#ifndef QT_NO_TOOLTIP
        pushBtnSrcDir->setToolTip(QApplication::translate("MainWindow", "\346\211\223\345\274\200\351\207\207\351\233\206\345\216\237\345\233\276\346\211\200\345\234\250\346\226\207\344\273\266\345\244\271", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnSrcDir->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\351\207\207\351\233\206\350\267\257\345\276\204", nullptr));
#ifndef QT_NO_TOOLTIP
        pushBtnObjDir->setToolTip(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\233\276\345\203\217\345\244\204\347\220\206\346\211\200\345\234\250\346\226\207\344\273\266\345\244\271", nullptr));
#endif // QT_NO_TOOLTIP
        pushBtnObjDir->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\244\204\347\220\206\350\267\257\345\276\204", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
