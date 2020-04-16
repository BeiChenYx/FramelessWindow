#include "custommodel.h"
#include <ctime>
#include <QDebug>

CustomModel::CustomModel(QAbstractTableModel *parent) : QAbstractTableModel(parent)
{
    auto now = QDateTime::currentDateTime();
    for (int i=0; i<100; ++i) {
        // typedef std::tuple<int, int, int, int, QString, QDateTime> ModeLCol;
        m_stocks.append({i, i*100, qrand() % 100, qrand() % 300, QString("name%1").arg(qrand() % 100), now.addSecs(i)});
    }

}

int CustomModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_stocks.length();
}
int CustomModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_names.length();
}

QVariant CustomModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        qDebug() << "index is valid: " << index.row() << ":" << index.column();
        return QVariant();
    }
    if(index.row() > m_stocks.length() || index.column() > m_names.length()){
        qDebug() << "m_stocks len: " << m_stocks.length() << " " << index.row() << ":" << index.column();
        return QVariant();
    }
    if(role == Qt::DisplayRole){
        auto stock = m_stocks.at(index.row());
        return (index.column() > stock.length() || index.column() < 0) ? QVariant() : stock.at(index.column());
    }
    return QVariant();
}

bool CustomModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid()){
        qDebug() << "index is valid: " << index.row() << ":" << index.column();
        return false;
    }
    if(role == Qt::EditRole){
        auto stock = m_stocks.at(index.row());
        stock[index.column()] = value;
        emit dataChanged(index, index);
        return true;
    }
    return false;
}

QVariant CustomModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole){
        return QVariant();
    }
    if(orientation == Qt::Horizontal){
        return QVariant(m_names.at(section));
    }else {
        return QVariant(QString::number(section));
    }
}
bool CustomModel::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if(role != Qt::EditRole){
        return false;
    }
    if(orientation == Qt::Horizontal){
        m_names.replace(section, value.toString());
        emit headerDataChanged(Qt::Horizontal, section, section);
        return true;
    }else{
        return false;
    }
}
