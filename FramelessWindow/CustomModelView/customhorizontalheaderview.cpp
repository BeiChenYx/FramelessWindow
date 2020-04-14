#include "customhorizontalheaderview.h"

CustomHorizontalHeaderView::CustomHorizontalHeaderView(QWidget *parent)
    : QHeaderView(Qt::Horizontal, parent)
{
    connect(this, &QHeaderView::sectionResized, this, &CustomHorizontalHeaderView::handleSectionResized);
    connect(this, &QHeaderView::sectionMoved, this, &CustomHorizontalHeaderView::handleSectionMoved);
    setSectionsMovable(true);
}

void CustomHorizontalHeaderView::showEvent(QShowEvent *e)
{
        for (int i=0; i<count(); ++i) {
            auto pTableFilter = new CustomHeaderView(i, this);
            pTableFilter->setGeometry(sectionViewportPosition(i), 0,
                                      sectionSize(i) - 5, height());
//            CustomModel *pCustomModel = dynamic_cast<CustomModel*>(this->model());
            pTableFilter->setTitle("");
            pTableFilter->show();
            m_pTableFilterList.append(pTableFilter);
            connect(pTableFilter, &CustomHeaderView::sortedUp, this, &CustomHorizontalHeaderView::sortedUp);
            connect(pTableFilter, &CustomHeaderView::sortedDown, this, &CustomHorizontalHeaderView::sortedDown);
            connect(pTableFilter, &CustomHeaderView::filter, this, &CustomHorizontalHeaderView::filter);
        }
        QHeaderView::showEvent(e);
}

void CustomHorizontalHeaderView::handleSectionResized(int i, int oldSize, int newSize)
{
    for (int j=visualIndex(i);j<count();j++) {
        int logical = logicalIndex(j);
        m_pTableFilterList.at(logical)->setGeometry(sectionViewportPosition(logical), 0,
                                                    sectionSize(logical) - 5, height());
    }
}
void CustomHorizontalHeaderView::handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex)
{
    for (int i=qMin(oldVisualIndex, newVisualIndex);i<count();i++){
        int logical = logicalIndex(i);
        m_pTableFilterList.at(logical)->setGeometry(sectionViewportPosition(logical), 0,
                                                    sectionSize(logical) - 5, height());
    }
}

void CustomHorizontalHeaderView::fixComboPositions()
{
    for (int i = 0; i<count();i++){
        m_pTableFilterList.at(i)->setGeometry(sectionViewportPosition(i), 0,
                                              sectionSize(i) - 5, height());
    }
}
