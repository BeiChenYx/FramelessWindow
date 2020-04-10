#include <QDebug>
#include "CustomModelView/customtableview.h"


CustomTableView::CustomTableView(QTableView *table, QWidget *parent)
    : QWidget(parent),
      m_pTableView(table)
{
    m_pSortFilterModel = new QSortFilterProxyModel(this);
}

void CustomTableView::initUi()
{
    m_pSortFilterModel->setSourceModel(m_pTableView->model());
    m_pSortFilterModel->setFilterKeyColumn(0);
    m_pTableView->setModel(m_pSortFilterModel);
    m_pHeaderView = m_pTableView->horizontalHeader();
    auto headModel = m_pHeaderView->model();
    // 给所有列添加自定义表头
    for (int i=0; i<headModel->columnCount(); ++i) {
        auto pTableFilter = new CustomHeaderView(i, this);
        m_pHeaderView->setIndexWidget(headModel->index(i, 0), pTableFilter);
        pTableFilter->move(m_pHeaderView->sectionPosition(i), 0);
        pTableFilter->resize(m_pHeaderView->sectionSize(i) - 5, m_pHeaderView->height());
        pTableFilter->setTitle(headModel->headerData(i, Qt::Horizontal).toString());
        connect(pTableFilter, &CustomHeaderView::sortedUp, this,[this](int index){
            m_pSortFilterModel->sort(index, Qt::AscendingOrder);
        });
        connect(pTableFilter, &CustomHeaderView::sortedDown, this,[this](int index){
            m_pSortFilterModel->sort(index, Qt::DescendingOrder);
        });
        connect(pTableFilter, &CustomHeaderView::filter, this,[this](int index){
            qDebug() << index;
        });

        m_pTableFilterList.append(pTableFilter);
        headModel->setHeaderData(i, Qt::Horizontal, "", Qt::EditRole);
    }
    connect(m_pHeaderView, &QHeaderView::sectionResized, this, [this](int logicalIndex, int oldSize, int newSize){
        Q_UNUSED(oldSize);
        for (int i=0; i<m_pTableFilterList.length(); ++i) {
            auto pTableFilter = m_pTableFilterList.at(i);
            if(i == logicalIndex){
                pTableFilter->resize(newSize - 5, m_pHeaderView->height());
            }
            pTableFilter->move(m_pHeaderView->sectionPosition(i), 0);
        }
    });
}
