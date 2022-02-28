#ifndef ARROWSBUTTON_H
#define ARROWSBUTTON_H

#include <QPushButton>

class ArrowsButton : public QPushButton
{
    Q_OBJECT
public:
    enum direction
    {
        left,
        right,
        front,
        back,
    };
    explicit ArrowsButton(/*int dir, QSize size = QSize(60, 40),*/ QWidget *parent = nullptr);
    void init(int dir /*, QSize size = QSize(60, 40)*/);

protected:
    void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
    //    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    bool eventFilter(QObject *watched, QEvent *event) Q_DECL_OVERRIDE;

private:
    QPainterPath m_path;
    int m_direction;
    bool m_hover = false;
    bool m_press = false;
};

#endif // ARROWSBUTTON_H
