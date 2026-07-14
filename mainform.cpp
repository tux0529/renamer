#include "mainform.h"
#include "ui_mainform.h"
#include "tablemodel.h"

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QUrl>
#include <QMimeData>
#include <QFileInfo>
#include <QTimer>
#include <QDebug>

MainForm::MainForm(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MainForm)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);

    ui->splitter->setStretchFactor(0,0);
    ui->splitter->setStretchFactor(1,1);

    ui->splitter->setCollapsible(0, false);

    int w = this->width();
    int w1 = 300;
    int w2 = w-2-300;

    ui->splitter->setSizes({w1, w2});

    m_tableModel = new TableModel(this);
    ui->tableView->setModel(m_tableModel);

    connect(ui->renameButton, &QPushButton::clicked, m_tableModel, &TableModel::rename);
    connect(ui->findLineEdit, &QLineEdit::textChanged, this, &MainForm::onFindLineEditTextChanged);
    connect(ui->replaceLineEdit, &QLineEdit::textChanged, this, &MainForm::onFindLineEditTextChanged);
    connect(ui->caseSensitiveCheckBox, &QCheckBox::stateChanged, this, &MainForm::onFindLineEditTextChanged);
    connect(ui->regxCheckBox, &QCheckBox::stateChanged, this, &MainForm::onFindLineEditTextChanged);

}

MainForm::~MainForm()
{
    delete ui;
}

void MainForm::onRenameButtonClicked()
{
    m_tableModel->insertRows(0, 8);
}

void MainForm::onFindLineEditTextChanged()
{
    //qDebug() << "QWidget: onFindLineEditTextChanged: findText: "<< ui->findLineEdit->text();
    QString replaceText = ui->replaceLineEdit->text();

    if(ui->regxCheckBox->isChecked()){

        QRegularExpression re(ui->findLineEdit->text(),
                              ui->caseSensitiveCheckBox->isChecked() ? QRegularExpression::NoPatternOption : QRegularExpression::CaseInsensitiveOption);

        m_tableModel->replace(re, replaceText);

    }
    else{
        QString findText = ui->findLineEdit->text();

        m_tableModel->replace(findText, replaceText, ui->caseSensitiveCheckBox->isChecked() ? Qt::CaseSensitive : Qt::CaseInsensitive);

    }

}

void MainForm::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasFormat("text/uri-list")){
        event->acceptProposedAction();
        return;
    }
    //qDebug() << "QWidget: drag entered";
    return QWidget::dragEnterEvent(event);
}

void MainForm::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();

    if(urls.size() < 1){

        return QWidget::dropEvent(event);
    }
    qDebug() << "QWidget: dropEvent: size" << urls.size();

    for (int i=0; i < urls.size(); ++i){
        m_tableModel->addFile(urls.at(i).toLocalFile());
    }

}
