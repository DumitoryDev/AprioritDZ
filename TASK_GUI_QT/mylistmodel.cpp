#include "mylistmodel.h"

MyListModel::MyListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

QVariant MyListModel::data(const QModelIndex& index, int role) const
{
    const size_t row = static_cast<size_t>(index.row());
    if ( !index.isValid() || index.row() < 0 || row >= m_data.size() )
    {
        return QVariant();
    }
    const QString& modelEntry = m_data[row];
    if (role == ImageSrc)
    {
        return modelEntry;
    }

    return QVariant();
}


int MyListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return static_cast<int>(m_data.size());

}

bool MyListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
         if (index.isValid() && role == Qt::EditRole) {

             m_data.push_back(value.toString());
             emit dataChanged(index, index);
             return true;
         }
         return false;
}



void MyListModel::add(const QString &str)
{
    beginInsertRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));   // kindly provided by superclass

    this->m_data.push_back(str);
    endInsertRows();
}

void MyListModel::delete_last_image()
{

    if(this->m_data.empty()){
        return;
    }
    this->m_data.pop_back();
    beginRemoveRows(QModelIndex(), rowCount(QModelIndex()), rowCount(QModelIndex()));   // kindly provided by superclass
    endRemoveRows();
}

void MyListModel::update_image(const QString &path)
{

    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
    dir.setSorting(QDir::Size | QDir::Reversed);
    QStringList filters;
    filters << "*.jpg" << "*.png" << "*.jpeg"<<"*PNG"<<"*.JPG";
    dir.setNameFilters(filters);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);
        QString qmlPath("file:///");
        qmlPath += fileInfo.filePath().replace("\\","//");

        this->add(qmlPath);

    }
}


QHash<int, QByteArray> MyListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[ImageSrc] = "imgSrc";

    return roles;
}

