#include <QDebug>
#include "CustomModelView/customtableview.h"

CustomTableView::CustomTableView(QWidget *parent) : QTableView(parent)
{
    m_pTBtnSort = new QToolButton();
    m_pTBtnSort->setIcon(QIcon(":/images/window.ico"));
    m_pTBtnSort->setText("");

    m_pSortFilterModel = new QSortFilterProxyModel(this);
    m_descendingOrder = false;
}

void CustomTableView::initUi()
{
    m_pSortFilterModel->setSourceModel(this->model());
    m_pSortFilterModel->setFilterKeyColumn(0);
    this->setModel(m_pSortFilterModel);
    m_pHeaderView = this->horizontalHeader();
    auto headModel = m_pHeaderView->model();
    m_pHeaderView->setIndexWidget(headModel->index(1, 0), m_pTBtnSort);
    m_pTBtnSort->setCursor(Qt::PointingHandCursor);
    m_pTBtnSort->move(m_pHeaderView->sectionPosition(1), 0);
    qDebug() << headModel->headerData(1, Qt::Horizontal)
             << " width: " << m_pHeaderView->sectionSize(1)
             << " height: " << m_pHeaderView->height();
    m_pTBtnSort->resize(m_pHeaderView->sectionSize(1), m_pHeaderView->height());
    connect(m_pTBtnSort, &QToolButton::clicked, this,[this](){
        qDebug() << "sort clieked";
        m_pSortFilterModel->sort(0, m_descendingOrder ? Qt::DescendingOrder : Qt::AscendingOrder);
        m_descendingOrder = !m_descendingOrder;
    });
    connect(m_pHeaderView, &QHeaderView::sectionResized, this, [this](int logicalIndex, int oldSize, int newSize){
        Q_UNUSED(oldSize);
        if(logicalIndex == 1){
            m_pTBtnSort->resize(newSize, m_pHeaderView->height());
        }
    });
}
