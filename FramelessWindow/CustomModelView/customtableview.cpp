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
}

void CustomTableView::setModel(QAbstractItemModel *model)
{
    m_pSortFilterModel->setSourceModel(model);
    QTableView::setModel(m_pSortFilterModel);
}
