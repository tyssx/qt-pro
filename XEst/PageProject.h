#ifndef PAGEPROJECT_H
#define PAGEPROJECT_H

#include <QFrame>

namespace Ui {
class PageProject;
}

class PageProject : public QFrame
{
    Q_OBJECT

public:
    explicit PageProject(QWidget *parent = nullptr);
    ~PageProject();

private:
    Ui::PageProject *ui;
};

#endif // PAGEPROJECT_H
