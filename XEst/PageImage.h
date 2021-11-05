#ifndef PAGEIMAGE_H
#define PAGEIMAGE_H

#include <QFrame>

namespace Ui {
class PageImage;
}

class PageImage : public QFrame
{
    Q_OBJECT

public:
    explicit PageImage(QWidget *parent = nullptr);
    ~PageImage();

private:
    Ui::PageImage *ui;
};

#endif // PAGEIMAGE_H
