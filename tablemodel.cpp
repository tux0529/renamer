#include "tablemodel.h"

#include <QColor>
#include <QRegularExpression>
#include <QRegularExpressionMatch>
#include <QDebug>

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel{parent}
{}

TableModel::~TableModel()
{

}

int TableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return this->m_fileList.size();
}

int TableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 2;
}

QVariant TableModel::data(const QModelIndex &index, int role) const
{

    if (!index.isValid())
        return QVariant();

    if (index.row() >= this->m_fileList.size() || index.row() < 0)
        return QVariant();

    auto &fi = this->m_fileList.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        if (index.column() == 0)
            return  fi->CurrentName;
        else if (index.column() == 1)
            return  fi->NewName.isEmpty()?"No Changed!":fi->NewName;
        break;
    case Qt::TextAlignmentRole:
        return Qt::AlignVCenter ;
        break;

    case Qt::ForegroundRole:
        if (index.column() == 1 && fi->NewName.isEmpty())
            return QVariant::fromValue(QColor(Qt::darkGray));

        break;

    default:
        break;
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
        case 0:
            return tr("Current Name");
            break;

        case 1:
            return tr("New Name");
            break;

        default:
            return QVariant();
        }
    }
    return QVariant();
}

bool TableModel::removeRows(int position, int rows, const QModelIndex &index)
{
    Q_UNUSED(index);
    beginRemoveRows(QModelIndex(), position, position + rows - 1);

    for (int row = 0; row < rows; ++row){
        auto fi = this->m_fileList.takeAt(position);
        fi->deleteLater();
    }

    endRemoveRows();
    return true;
}

bool TableModel::insertRow(int row, const QModelIndex &parent)
{
    return insertRows(row, 1, QModelIndex());
}

bool TableModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(QModelIndex(), row, row + count -1);

    for (int i = 0; i < count; ++i){
        FileInfo *fi = new FileInfo(QString(), this);
        m_fileList.insert(i+row, fi);
    }

    endInsertRows();
    return true;
}



bool TableModel::clear()
{
    beginResetModel();
    for(int i = m_fileList.size(); i >0; --i){
        auto fi = this->m_fileList.takeLast();
        fi->deleteLater();
    }
    endResetModel();
    return true;
}

void TableModel::addFile(const QString &filepath)
{
    int index = m_fileList.size();
    beginInsertRows(QModelIndex(), index, index);

    FileInfo *fi = new FileInfo(filepath, this);

    m_fileList.append(fi);

    endInsertRows();
}

void TableModel::replace(const QString &findText, const QString &replaceText, Qt::CaseSensitivity cs)
{
    for(int i = m_fileList.size()-1; i>-1; --i){
        auto fi = m_fileList.at(i);
        //qDebug() << "TableModel::replace: Path: "<< fi->Path;

        if( findText.isEmpty() || (0 > fi->CurrentName.indexOf(findText, 0, cs)) ){
            if(!(fi->NewName.isEmpty())){
                fi->NewName = QString("");
            }
        }
        else{
            QString nn = fi->CurrentName;
            fi->NewName = nn.replace(findText, replaceText, cs);
        }
    }

    emit dataChanged( this->createIndex(0, 1), this->createIndex(m_fileList.size() -1, 1));
}

void TableModel::replace(const QRegularExpression &findRE, const QString &replaceText)
{
    for(int i = m_fileList.size()-1; i>-1; --i){
        auto fi = m_fileList.at(i);

        QRegularExpressionMatch match = findRE.match(fi->CurrentName);
        if (findRE.isValid() && match.hasMatch()) {
            QString findText = match.captured(0);
            QStringList capList = match.capturedTexts();
            QString replaceTextMatch = replaceText;
            for(int i = capList.size() -1; i > 0 ; --i){
                //qDebug() << "TableModel::replace: index: "<< i;
                replaceTextMatch.replace(QString("$%1").arg(i), capList.at(i));
            }
            QString nn = fi->CurrentName;
            fi->NewName = nn.replace(findText, replaceTextMatch);
        }
        else{
            if(!(fi->NewName.isEmpty())){
                fi->NewName = QString("");
            }
        }

    }

    emit dataChanged( this->createIndex(0, 1), this->createIndex(m_fileList.size() -1, 1));

}

void TableModel::rename()
{

    for(int i = m_fileList.size()-1; i>-1; --i){
        auto fi = m_fileList.at(i);

        if(!(fi->NewName.isEmpty())){
            QString newFileName = fi->Path + fi->NewName;
            QString oldFileName = fi->Path + fi->CurrentName;
            if (QFile::rename(oldFileName, newFileName) ){
                fi->CurrentName = fi->NewName;
                fi->NewName = QString("");
            }
        }
    }
    emit dataChanged( this->createIndex(0, 0), this->createIndex(m_fileList.size() -1, 1));
}

