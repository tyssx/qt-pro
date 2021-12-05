#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QTextEdit;
class QLabel;
class QPrinter;
class QProgressBar;
class QPlainTextEdit;
class VersionDialog;
class WordWidget;
class QSpinBox;
class QFontComboBox;
class ColorSetDialog;
class TimeDateSetDialog;
class ListWidgets;
class TreeWidgets;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void myMenuBar();
    void myToolBar();
    bool eventFilter(QObject *watched, QEvent *event);
    void dockShowContents(QString contents);

private slots:
    void slot_openFile();
    void slot_closeFile();
    void slot_saveFile();
    void slot_printDocument();
    void slot_printPreview();

private:
    enum DuplexMode
    {
        DuplexNone = 0,
        DuplexAuto,
        DuplexLongSide,
        DuplexShortSide
    };
    QTextEdit *textEdit;
    QLabel *labelFile;
    QLabel *labelByte;
    QPrinter *m_printer;
    QProgressBar *m_progress;
    QSpinBox *m_spinFontSize;
    QFontComboBox *m_fontComboBox;
    QTimer *timer;

    QDockWidget *dockWidget;
    VersionDialog *m_versionDialog;
    WordWidget *m_wordWidget;
    ColorSetDialog *m_colorSetDialog;
    TimeDateSetDialog *m_timeDateSetDialog;
    //    ListWidgets *m_listWidgets;
    //    TreeWidgets *m_treeWidgets;
    QString curPathname;
    QPlainTextEdit *plainText;
    int a = 0;
};
#endif // MAINWINDOW_H
