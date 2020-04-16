#include <QDebug>
#include <QLabel>
#include "customhorizontalheaderview.h"

const int HEADER_RIGHT_BORDER = 1;

CustomHorizontalHeaderView::CustomHorizontalHeaderView(QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent)
{
    connect(this, &QHeaderView::sectionResized, this, &CustomHorizontalHeaderView::handleSectionResized);
    connect(this, &QHeaderView::sectionMoved, this, &CustomHorizontalHeaderView::handleSectionMoved);
    setSectionsMovable(true);
    setSectionsClickable(true);
    this->setStretchLastSection(true);
    this->setObjectName("CustomHorizontalHeaderView");
}
CustomHorizontalHeaderView::~CustomHorizontalHeaderView()
{
    for (auto tableFilter : m_pTableFilterList) {
        delete tableFilter;
        tableFilter = nullptr;
    }
}

void CustomHorizontalHeaderView::showEvent(QShowEvent *e)
{
    if(!m_pTableFilterList.isEmpty()){return;}
    for (int i=0; i<count(); ++i) {
        auto pTableFilter = new CustomHeaderView(i, this);
        pTableFilter->setGeometry(sectionViewportPosition(i), 0,
                                  sectionSize(i) - HEADER_RIGHT_BORDER, height());
        pTableFilter->setTitle(this->model()->headerData(i, Qt::Horizontal).toString());
        pTableFilter->show();
        m_pTableFilterList.append(pTableFilter);
        connect(pTableFilter, &CustomHeaderView::sortedUp, this, [this](int index){
            emit this->sortedUp(index);
            for (int i=0; i<m_pTableFilterList.length(); ++i) {
                if(i == index){continue;}
                m_pTableFilterList.at(index)->clearStatus();
            }
        });
        connect(pTableFilter, &CustomHeaderView::sortedDown, this, [this](int index){
            m_pTableFilterList.at(index)->clearStatus();
            emit this->sortedDown(index);
            for (int i=0; i<m_pTableFilterList.length(); ++i) {
                if(i == index){continue;}
                m_pTableFilterList.at(index)->clearStatus();
            }
        });
        connect(pTableFilter, &CustomHeaderView::filter, this, [this](int index, QString msg){
            if(m_pTableFilterList[index]->getFilterMsg().trimmed().isEmpty()){
                m_pTableFilterList[index]->clearFilterStatus();
            }
            emit this->filter(index, msg);
        });
    }
    QHeaderView::showEvent(e);
}
void CustomHorizontalHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    // 屏蔽默认绘制表头，避免修改model, model 可以不更改情况下共享给不同的view
    // 表头内容都在showEvent中处理，目前没有其他更好方法
    Q_UNUSED(rect);
    painter->save();
    QHeaderView::paintSection(painter, QRect(), logicalIndex);
    painter->restore();
}

void CustomHorizontalHeaderView::handleSectionResized(int i, int oldSize, int newSize)
{
    Q_UNUSED(oldSize);
    Q_UNUSED(newSize);
    for (int j=visualIndex(i);j<count();j++) {
        int logical = logicalIndex(j);
        m_pTableFilterList.at(logical)->setGeometry(sectionViewportPosition(logical), 0,
                                                    sectionSize(logical) - HEADER_RIGHT_BORDER, height());
    }
}
void CustomHorizontalHeaderView::handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logical);
    for (int i=qMin(oldVisualIndex, newVisualIndex);i<count();i++){
        int logical = logicalIndex(i);
        m_pTableFilterList.at(logical)->setGeometry(sectionViewportPosition(logical), 0,
                                                    sectionSize(logical) - HEADER_RIGHT_BORDER, height());
    }
}
void CustomHorizontalHeaderView::resizeEvent(QResizeEvent *event)
{
    QHeaderView::resizeEvent(event);
    for (int i=0;i<m_pTableFilterList.length();i++){
        int logical = logicalIndex(i);
        m_pTableFilterList.at(logical)->setGeometry(sectionViewportPosition(logical), 0,
                                                    sectionSize(logical) - HEADER_RIGHT_BORDER, height());
    }
}

void CustomHorizontalHeaderView::fixComboPositions()
{
    for (int i = 0; i<count();i++){
        m_pTableFilterList.at(i)->setGeometry(sectionViewportPosition(i), 0,
                                              sectionSize(i) - HEADER_RIGHT_BORDER, height());
    }
}
