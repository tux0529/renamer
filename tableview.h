#ifndef TRACKTABLEVIEW_H
#define TRACKTABLEVIEW_H

#include <QTableView>

class QMenu;

class TableView : public QTableView
{
    Q_OBJECT
public:
    TableView(QWidget *parent = nullptr);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;

private:
    QMenu *m_menu;
    QAction *m_removeAction;
    QAction *m_clearAction;
};

#endif // TRACKTABLEVIEW_H
