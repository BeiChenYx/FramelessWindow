#include <QDebug>
#include <QItemDelegate>
#include "CustomModelView/customtableview.h"


CustomTableView::CustomTableView(QWidget *parent)
    : QTableView(parent),
      m_pHHeaderView(new CustomHorizontalHeaderView(this)),
      m_pSortFilterModel(new QSortFilterProxyModel(this))
{
    this->initConnect();
    this->setAlternatingRowColors(true);
    this->setHorizontalHeader(m_pHHeaderView);
    this->setObjectName("CustomTableView");
}

void CustomTableView::initConnect()
{

    connect(m_pHHeaderView, &CustomHorizontalHeaderView::sortedUp, this, [this](int index){
        m_pSortFilterModel->setFilterKeyColumn(index);
        m_pSortFilterModel->sort(index, Qt::AscendingOrder);
    });
    connect(m_pHHeaderView, &CustomHorizontalHeaderView::sortedDown, this, [this](int index){
        m_pSortFilterModel->setFilterKeyColumn(index);
        m_pSortFilterModel->sort(index, Qt::DescendingOrder);
    });
    connect(m_pHHeaderView, &CustomHorizontalHeaderView::filter, this, [this](int index, QString msg){
        m_pSortFilterModel->setFilterKeyColumn(index);
        m_pSortFilterModel->setFilterFixedString(msg);
    });

    // 设置选中行
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    // 设置单选
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    // 处理鼠标选择
    connect(this, &QTableView::pressed, this, [this](const QModelIndex &index){
        qDebug() << "selected: " << index.row() << " data:" << this->model()->data(index).toString();
    });
}

void CustomTableView::setModel(QAbstractItemModel *model)
{
    m_pSortFilterModel->setSourceModel(model);
    QTableView::setModel(m_pSortFilterModel);
}
