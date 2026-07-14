#include "tableview.h"
#include "tablemodel.h"

#include <QHeaderView>
#include <QMenu>
#include <QContextMenuEvent>
#include <QDebug>

TableView::TableView(QWidget *parent) :
    QTableView(parent)
    , m_menu(new QMenu(this))
{

    this->setShowGrid(false);

    this->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    this->horizontalHeader()->setMinimumSectionSize(40);
    this->verticalHeader()->hide();
    this->verticalHeader()->setDefaultSectionSize(27);
    this->setAlternatingRowColors(true);

    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);

    this->setContextMenuPolicy(Qt::DefaultContextMenu);

    m_clearAction = new QAction(tr("Empty List"), this);
    m_removeAction = new QAction(tr("Remove"), this);
    m_menu->addAction(m_clearAction);
    m_menu->addAction(m_removeAction);
    connect(m_clearAction, &QAction::triggered, this, [=](){
        dynamic_cast<TableModel*> (this->model())->clear();
    });
    connect(m_removeAction, &QAction::triggered, this, [=](){
        //QModelIndexList selectedIndexes = this->selectedIndexes();
        QModelIndexList selectedIndexes = this->selectionModel()->selectedRows();
        QList<int> list;
        foreach (const QModelIndex &index, selectedIndexes) {
            list.append(index.row());
        }
        std::sort(list.begin(), list.end());

        while (!list.isEmpty()){
            //qDebug() << list.takeLast();
            dynamic_cast<TableModel*> (this->model())->removeRows(list.takeLast(),1);
        }

    });
}

void TableView::resizeEvent(QResizeEvent *event)
{

    Q_UNUSED(event)

    //qDebug("resizeEvent");
    int w = this->width();

    this->setColumnWidth(0, w/2-4);
    this->setColumnWidth(1, w/2-4);
}

void TableView::contextMenuEvent(QContextMenuEvent *event)
{
    if (indexAt(event->pos()).isValid()) {
        m_removeAction->setEnabled(true);
    }
    else{
        m_removeAction->setEnabled(false);
    }
    m_menu->exec(event->globalPos());
}
