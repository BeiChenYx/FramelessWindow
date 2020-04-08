#include <QDebug>
#include "CustomModelView/customtableview.h"

CustomTableView::CustomTableView(QWidget *parent) : QTableView(parent)
{
}

void CustomTableView::initUi()
{
    m_pTBtnSort = new QToolButton();
    m_pTBtnSort->setIcon(QIcon(":/images/window.ico"));
    m_pTBtnSort->setText("");
//    auto model = this->model();
//    this->horizontalHeader()->setIndexWidget(model->index(0, 0), m_pTBtnSort);
    m_pHeaderView = this->horizontalHeader();
    auto headModel = m_pHeaderView->model();
    m_pHeaderView->setIndexWidget(headModel->index(1, 0), m_pTBtnSort);
    m_pTBtnSort->setCursor(Qt::PointingHandCursor);
//    pHheader->moveSection(pHheader->sectionPosition(1), 0);
    m_pTBtnSort->move(m_pHeaderView->sectionPosition(1), 0);
    qDebug() << headModel->headerData(1, Qt::Horizontal)
             << " width: " << m_pHeaderView->sectionSize(1)
             << " height: " << m_pHeaderView->height();
    m_pTBtnSort->resize(m_pHeaderView->sectionSize(1), m_pHeaderView->height());
    connect(m_pTBtnSort, &QToolButton::clicked, this,[](){
        qDebug() << "sort clieked";
    });
    connect(m_pHeaderView, &QHeaderView::sectionResized, this, [this](int logicalIndex, int oldSize, int newSize){
        Q_UNUSED(oldSize);
        if(logicalIndex == 1){
            m_pTBtnSort->resize(newSize, m_pHeaderView->height());
        }
    });
}
