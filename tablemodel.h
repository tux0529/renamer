#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QDir>

class FileInfo : public QObject{
    Q_OBJECT
public:
    explicit FileInfo(const QString &filepath, QObject *parent = nullptr){
        QString fp = QDir::fromNativeSeparators(filepath);
        int index = fp.lastIndexOf("/") +1;
        Path = fp.left(index);
        CurrentName = fp.mid(index);
    };
    QString CurrentName;
    QString NewName;
    QString Path;
};

typedef QList<FileInfo*> FileList;


class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TableModel(QObject *parent = nullptr);
    ~TableModel();

    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    bool removeRows(int position, int rows, const QModelIndex &index = QModelIndex()) override;
    bool insertRow(int row, const QModelIndex &parent = QModelIndex());
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool clear();

    void addFile(const QString &filepath);

    void replace(const QString &findText, const QString &replaceText, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    void replace(const QRegularExpression &findRE, const QString &replaceText);

    void rename();

private:
    FileList m_fileList;
};

#endif // TABLEMODEL_H
