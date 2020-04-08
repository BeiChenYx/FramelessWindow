#ifndef CUSTOMMODEL_H
#define CUSTOMMODEL_H

#include <QObject>
#include <QWidget>
#include <QVector>
#include <QList>
#include <QDateTime>
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QAbstractTableModel>
#include <tuple>
#include <functional>

//typedef std::tuple<int, int, int, int, QString, QDateTime> ModelCol;

//class CustomModel : public QAbstractItemModel
//class CustomModel : public QAbstractListModel
class CustomModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CustomModel(QAbstractTableModel *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
signals:

public slots:

private:
// 列: code, no1, no2, no3, name, datetime
    QStringList m_names{"code", "no1", "no2", "no3", "name", "datetime"};
    QList<QVector<QVariant> > m_stocks;
};

#endif // CUSTOMMODEL_H
