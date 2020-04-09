#include <QDebug>
#include "CustomModelView/customtableview.h"

TableHeaderFilter::TableHeaderFilter(int index, QWidget *parent)
    : QWidget(parent),
      m_index(index)
{
    m_pTitle = new QLabel(this);
    m_pTitle->setStyleSheet("color: red;");
    m_pFilter = new QToolButton(this);
    m_pFilter->setIcon(QIcon(":/images/filter.png"));
    m_pFilter->setCursor(Qt::PointingHandCursor);
    m_pSorterUp = new QToolButton(this);
    m_pSorterDown = new QToolButton(this);
    m_pSorterUp->setIcon(QIcon(":/images/sort_up.png"));
    m_pSorterDown->setIcon(QIcon(":/images/sort_down.png"));
    m_pSorterUp->setCursor(Qt::PointingHandCursor);
    m_pSorterDown->setCursor(Qt::PointingHandCursor);
    QHBoxLayout *pHLayout = new QHBoxLayout();
    pHLayout->addWidget(m_pTitle);
    pHLayout->addWidget(m_pFilter);
    QVBoxLayout *pVLayout = new QVBoxLayout();
    pVLayout->addWidget(m_pSorterUp);
    pVLayout->addWidget(m_pSorterDown);
    pHLayout->addLayout(pVLayout);
    pHLayout->setMargin(0);
    pVLayout->setMargin(0);
    pHLayout->setSpacing(0);
    pVLayout->setSpacing(0);
    pVLayout->setContentsMargins(0, 0, 0, 0);
    pHLayout->setContentsMargins(5, 0, 5, 0);
    this->setLayout(pHLayout);
    connect(m_pSorterUp, &QToolButton::clicked, this, &TableHeaderFilter::sortedUp);
    connect(m_pSorterDown, &QToolButton::clicked, this, &TableHeaderFilter::sortedDown);
}

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
        auto pTableFilter = new TableHeaderFilter(i, this);
        m_pHeaderView->setIndexWidget(headModel->index(i, 0), pTableFilter);
        pTableFilter->move(m_pHeaderView->sectionPosition(i), 0);
        pTableFilter->resize(m_pHeaderView->sectionSize(i), m_pHeaderView->height());
        pTableFilter->setText(headModel->headerData(i, Qt::Horizontal).toString());
        connect(pTableFilter, &TableHeaderFilter::sortedUp, this,[this](){
            m_pSortFilterModel->sort(0, Qt::AscendingOrder);
        });
        connect(pTableFilter, &TableHeaderFilter::sortedDown, this,[this](){
            m_pSortFilterModel->sort(0, Qt::DescendingOrder);
        });
        m_pTableFilterList.append(pTableFilter);
        bool re = headModel->setHeaderData(i, Qt::Horizontal, "", Qt::DisplayRole);
        re = headModel->setHeaderData(i, Qt::Horizontal, "", Qt::EditRole);
        qDebug() << re;
    }
    connect(m_pHeaderView, &QHeaderView::sectionResized, this, [this](int logicalIndex, int oldSize, int newSize){
        Q_UNUSED(oldSize);
        for (int i=0; i<m_pTableFilterList.length(); ++i) {
            auto pTableFilter = m_pTableFilterList.at(i);
            if(i == logicalIndex){
                pTableFilter->resize(newSize, m_pHeaderView->height());
            }
            pTableFilter->move(m_pHeaderView->sectionPosition(i), 0);
        }
    });
}
