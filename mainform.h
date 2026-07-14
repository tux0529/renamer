#ifndef MAINFORM_H
#define MAINFORM_H


#include <QWidget>


class TableModel;

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = nullptr);
    ~MainForm();

public slots:
    void onRenameButtonClicked();
    void onFindLineEditTextChanged();

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Ui::MainForm *ui;

    TableModel *m_tableModel;
};

#endif // MAINFORM_H
