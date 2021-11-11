#include "wordwidget.h"

#include "ui_wordwidget.h"

#include <QVBoxLayout>

WordWidget::WordWidget(QWidget *parent) : QWidget(parent), ui(new Ui::WordWidget)
{
    ui->setupUi(this);
    //垂直布局
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(ui->groupBoxForm);
    vLayout->addWidget(ui->groupBoxColor);
    vLayout->addWidget(ui->plainTextEdit);
    //需要添加一个widget，用来存放水平布局，直接添加布局得不到正确布局
    QWidget *w = new QWidget;
    w->setLayout(ui->horizontalLayout_3);
    vLayout->addWidget(w);
    setLayout(vLayout);

    connect(ui->checkBoxUnderline, &QCheckBox::stateChanged, this, &WordWidget::slot_changeForm);
    connect(ui->checkBoxItalic, &QCheckBox::stateChanged, this, &WordWidget::slot_changeForm);
    connect(ui->checkBoxBold, &QCheckBox::stateChanged, this, &WordWidget::slot_changeForm);

    connect(ui->radioButtonRed, &QRadioButton::clicked, this, &WordWidget::slot_changeColor);
    connect(ui->radioButtonBlue, &QRadioButton::clicked, this, &WordWidget::slot_changeColor);
    connect(ui->radioButtonBlack, &QRadioButton::clicked, this, &WordWidget::slot_changeColor);

    connect(ui->pushButtonOk, &QPushButton::clicked, this, &WordWidget::close);
    connect(ui->pushButtonCancel, &QPushButton::clicked, this, &WordWidget::close);
}

WordWidget::~WordWidget()
{
    delete ui;
}

void WordWidget::slot_changeForm(int status)
{
    QCheckBox *check = qobject_cast<QCheckBox *>(sender());
    QFont font       = ui->plainTextEdit->font();
    if (check == ui->checkBoxUnderline) { font.setUnderline(status); }

    else if (check == ui->checkBoxItalic)
    {
        font.setItalic(status);
    }
    else if (check == ui->checkBoxBold)
    {
        font.setBold(status);
    }
    ui->plainTextEdit->setFont(font);
}

void WordWidget::slot_changeColor()
{
    QRadioButton *rBtn = qobject_cast<QRadioButton *>(sender());
    QPalette palette   = ui->plainTextEdit->palette();
    if (rBtn == ui->radioButtonRed) { palette.setColor(QPalette::Text, Qt::red); }
    else if (rBtn == ui->radioButtonBlue)
    {
        palette.setColor(QPalette::Text, Qt::blue);
    }
    else if (rBtn == ui->radioButtonBlack)
    {
        palette.setColor(QPalette::Text, Qt::black);
    }
    ui->plainTextEdit->setPalette(palette);
    //或
    //    QPalette palette = ui->plainTextEdit->palette();
    //    if (ui->radioButtonRed->isChecked()) { palette.setColor(QPalette::Text, Qt::red); }
    //    else if (ui->radioButtonBlue->isChecked())
    //    {
    //        palette.setColor(QPalette::Text, Qt::blue);
    //    }
    //    else if (ui->radioButtonBlack->isChecked())
    //    {
    //        palette.setColor(QPalette::Text, Qt::black);
    //    }
    //    ui->plainTextEdit->setPalette(palette);
}
