#pragma once

#include <QAbstractListModel>
#include <QObject>
#include <vector>
#include <QDir>
#include <QDebug>
#include <QUrl>

enum DataRoles
{
    ImageSrc = Qt::UserRole + 1,

};


class MyListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    MyListModel(QObject *parent = nullptr);


    Q_INVOKABLE virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE virtual int rowCount(const QModelIndex &parent) const override;
    Q_INVOKABLE virtual bool setData(const QModelIndex &index,const QVariant &value, int role) override;



    Q_INVOKABLE void add(const QString & str);
    Q_INVOKABLE void delete_last_image(void);
    Q_INVOKABLE void update_image(const QString & path);




    virtual QHash<int,QByteArray> roleNames() const override;

private:

    std::vector<QString> m_data;
};
