#include "maskeditor.h"

#include "maskindicator.h"
#include "ui_maskeditor.h"

#include <QLabel>
#include <QUndoStack>

#if _MSC_VER >= 1600
#    pragma execution_character_set("utf-8")
#endif

MaskEditor::MaskEditor(QWidget *parent) : QMainWindow(parent), ui(new Ui::MaskEditor)
{
    ui->setupUi(this);
    m_labelInfo = new QLabel;
    statusBar()->addWidget(m_labelInfo);

    //工具分组
    m_actionGroupTool = new QActionGroup(this);
    m_actionGroupTool->addAction(ui->actionRect);
    m_actionGroupTool->addAction(ui->actionEllipse);
    m_actionGroupTool->addAction(ui->actionPolygon);
    ui->actionRect->setData(MaskIndicator::Rectangle);
    ui->actionEllipse->setData(MaskIndicator::Ellipse);
    ui->actionPolygon->setData(MaskIndicator::Polygon);
    ui->actionEllipse->setChecked(true);
    connect(m_actionGroupTool, &QActionGroup::triggered, this, [&](QAction *action) {
        slot_maskMerge();
        //        m_scene->updateIndicator(action->data().toInt());
    });
    m_actionGroupOperate = new QActionGroup(this);
    m_actionGroupOperate->addAction(ui->actionNew);
    m_actionGroupOperate->addAction(ui->actionAdd);
    m_actionGroupOperate->addAction(ui->actionSubtract);
    m_actionGroupOperate->addAction(ui->actionIntersect);
    ui->actionNew->setData(0);
    ui->actionAdd->setData(1);
    ui->actionSubtract->setData(2);
    ui->actionIntersect->setData(3);

    static QVector<QCursor> cursors;
    if (cursors.isEmpty())
    {
        cursors << QCursor(QPixmap(":/images/operate_new.png"), 9, 9);
        cursors << QCursor(QPixmap(":/images/operate_add.png"), 9, 9);
        cursors << QCursor(QPixmap(":/images/operate_subtract.png"), 9, 9);
        cursors << QCursor(QPixmap(":/images/operate_intersect.png"), 9, 9);
    }
    connect(m_actionGroupOperate, &QActionGroup::triggered, this, [&](QAction *action) {
        QCursor cursor = cursors.at(action->data().toInt());
        ui->graphicsView->viewport()->setCursor(cursor);
    });
    ui->actionAdd->setChecked(true);
    ui->graphicsView->viewport()->setCursor(cursors.at(1));

    //场景，图像,dataItem及MASK
    m_scene = new MaskScene;
    ui->graphicsView->setScene(m_scene);
    ui->graphicsView->setZoomButtonVisible(false);
}

MaskEditor::~MaskEditor()
{
    delete ui;
}

void MaskEditor::on_actionClear_triggered()
{
    //    m_stack->push(new MaskCommand({}, m_document));
    //    m_stack->clear();
}

void MaskEditor::on_actionReverse_triggered()
{
    //    QPainterPath bound;
    //    bound.addPolygon(m_polygon);

    //    QPainterPath cur = m_document->path();

    //    m_stack->push(new MaskCommand(bound - cur, m_document));
}

void MaskEditor::slot_maskMerge()
{
}

void MaskEditor::slot_indicatorChanged()
{
}
