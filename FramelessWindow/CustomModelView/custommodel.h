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
#include <QStandardItemModel>
#include <tuple>
#include <functional>

//typedef std::tuple<int, int, int, int, QString, QDateTime> ModelCol;

class CustomModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit CustomModel(QAbstractItemModel *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

signals:

public slots:

private:
    // 列: code, no1, no2, no3, name, datetime
    QVector<QString> m_names{"code", "no1", "no2", "no3", "name", "datetime"};
    QList<QVector<QVariant> > m_stocks;
};

#endif // CUSTOMMODEL_H
