#ifndef PAGERUN_H
#define PAGERUN_H

#include <QFrame>

namespace Ui {
class PageRun;
}

class PageRun : public QFrame
{
    Q_OBJECT

public:
    explicit PageRun(QWidget *parent = nullptr);
    ~PageRun();

private:
    Ui::PageRun *ui;
};

#endif // PAGERUN_H
