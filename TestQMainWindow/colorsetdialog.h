#ifndef COLORSETDIALOG_H
#define COLORSETDIALOG_H

#include <QDialog>

namespace Ui
{
class ColorSetDialog;
}

class ColorSetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ColorSetDialog(QWidget *parent = nullptr);
    ~ColorSetDialog();

private slots:
    void slot_colorSliderChanged();
    void slot_radioButtonClicked();

    void on_dial_valueChanged(int value);

    void on_horizontalSlider_valueChanged(int value);

    void on_horizontalScrollBar_valueChanged(int value);

    void on_verticalSlider_valueChanged(int value);

    void on_verticalScrollBar_valueChanged(int value);

private:
    Ui::ColorSetDialog *ui;
};

#endif // COLORSETDIALOG_H
