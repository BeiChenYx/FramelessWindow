#include <QDebug>
#include <QLabel>
#include "customhorizontalheaderview.h"

const int HEADER_RIGHT_BORDER = 1;

CustomHorizontalHeaderView::CustomHorizontalHeaderView(QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent)
{
    setSectionsMovable(true);
    setSectionsClickable(true);
    this->setStretchLastSection(true);
    this->setObjectName("CustomHorizontalHeaderView");
    connect(this, &QHeaderView::sectionResized, this, &CustomHorizontalHeaderView::handleSectionResized);
    connect(this, &QHeaderView::sectionMoved, this, &CustomHorizontalHeaderView::handleSectionMoved);
}
CustomHorizontalHeaderView::~CustomHorizontalHeaderView()
{
    for (auto tableFilter : m_pTableFilterList) {
        delete tableFilter;
        tableFilter = nullptr;
    }
}

void CustomHorizontalHeaderView::setModel(QAbstractItemModel *model)
{
    QHeaderView::setModel(model);
    for (int i=0; i<count(); ++i) {
        auto pTableFilter = new CustomHeaderView(i, this);
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
}

void CustomHorizontalHeaderView::headerDataChanged(Qt::Orientation orientation, int logicalFirst, int logicalLast)
{
    if(logicalFirst < 0 || logicalLast > m_pTableFilterList.length()){return;}
    if(orientation == Qt::Horizontal){
        for (int i=logicalFirst; i<=logicalLast; ++i) {
            m_pTableFilterList.at(i)->setTitle(this->model()->headerData(i, Qt::Horizontal).toString());
        }
    }
}
void CustomHorizontalHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
    // 屏蔽默认绘制表头，避免修改model, model 可以不更改情况下共享给不同的view
    // 表头内容都在showEvent中处理，目前没有其他更好方法
    Q_UNUSED(rect);
    painter->save();
    QHeaderView::paintSection(painter, QRect(), logicalIndex);
    painter->restore();
    auto pTableFilter = m_pTableFilterList.at(logicalIndex);
    pTableFilter->setGeometry(sectionViewportPosition(logicalIndex), 0,
                              sectionSize(logicalIndex) - HEADER_RIGHT_BORDER, height());
    pTableFilter->show();
    // 根据第一个可见视图索引求出逻辑索引, 在这之前的都要隐藏
    int startShowIndex = this->visualIndexAt(0);
    for (int i=0; i<startShowIndex; ++i) {
        m_pTableFilterList.at(i)->hide();
    }
}

void CustomHorizontalHeaderView::handleSectionResized(int i, int oldSize, int newSize)
{
    Q_UNUSED(i)
    Q_UNUSED(oldSize);
    Q_UNUSED(newSize);
    this->fixSectionPositions();
}
void CustomHorizontalHeaderView::handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex)
{
    Q_UNUSED(logical);
    Q_UNUSED(oldVisualIndex);
    Q_UNUSED(newVisualIndex);
    this->fixSectionPositions();
}
void CustomHorizontalHeaderView::resizeEvent(QResizeEvent *event)
{
    QHeaderView::resizeEvent(event);
    this->fixSectionPositions();
}

void CustomHorizontalHeaderView::fixSectionPositions()
{
    for (int i = 0; i<m_pTableFilterList.length();i++){
        m_pTableFilterList.at(i)->setGeometry(sectionViewportPosition(i), 0,
                                              sectionSize(i) - HEADER_RIGHT_BORDER, height());
    }
}
