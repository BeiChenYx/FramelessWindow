#include "custommodel.h"
#include <ctime>
#include <algorithm>
#include <QDebug>

CustomModel::CustomModel(QAbstractItemModel *parent) : QAbstractItemModel(parent)
{
//    std::list<int> intList;
    auto now = QDateTime::currentDateTime();
    for (int i=0; i<10; ++i) {
        // typedef std::tuple<int, int, int, int, QString, QDateTime> ModeLCol;
        m_stocks.append({i, i*100, qrand() % 100, qrand() % 300, QString("name%1\nname_name").arg(qrand() % 100), now.addSecs(i)});
//        auto item = qrand() % 100;
//        intList.push_back(item);
//        qDebug() << item;
    }
//    auto listMin = std::min_element(intList.begin(), intList.end());
//    auto listMax = std::max_element(intList.begin(), intList.end());
//    qDebug() << *listMin << " : " << *listMax;
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
    } else if(role == Qt::EditRole){
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
        if(section < 0 || section > m_names.length()){return  QVariant();}
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

Qt::ItemFlags CustomModel::flags(const QModelIndex &index) const
{
    if(index.isValid()){
         Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemNeverHasChildren | Qt::ItemIsEditable;
         return flags;
    }
    return QAbstractItemModel::flags(index);
}

QModelIndex CustomModel::index(int row, int column, const QModelIndex &parent) const
{
    if (row < 0 || column < 0 || column >= columnCount(parent) || column > m_names.length())
        return QModelIndex();

    return createIndex(row,column);
}

QModelIndex CustomModel::parent(const QModelIndex &child) const
{
    Q_UNUSED(child);
    return QModelIndex();
}

void CustomModel::sort(int column, Qt::SortOrder order)
{
    // 修改之前发生信号
    emit layoutAboutToBeChanged();
    if(column <= 4 && column >= 0){
        if(order == Qt::AscendingOrder){
            std::sort(m_stocks.begin(), m_stocks.end(), [](QVector<QVariant> pre, QVector<QVariant> item){
                return pre.at(2).toInt() < item.at(2).toInt();
            });
        }
        if(order == Qt::DescendingOrder){
            std::sort(m_stocks.rbegin(), m_stocks.rend(), [](QVector<QVariant> pre, QVector<QVariant> item){
                return pre.at(2).toInt() < item.at(2).toInt();
            });
        }
    }
    // 修改之后要发送的信号
    emit layoutChanged();
}
