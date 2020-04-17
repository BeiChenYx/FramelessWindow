#ifndef CUSTOMHORIZONTALHEADERVIEW_H
#define CUSTOMHORIZONTALHEADERVIEW_H

#include <QObject>
#include <QHeaderView>
#include <QPainter>

#include "customheaderview.h"

class CustomHorizontalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit CustomHorizontalHeaderView(QWidget *parent = nullptr);
    virtual ~CustomHorizontalHeaderView() override;

signals:
    void sortedUp(int index);
    void sortedDown(int index);
    void filter(int index, QString msg);

public slots:

protected:
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
    void resizeEvent(QResizeEvent *event) override;
    void handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex);
    void handleSectionResized(int i, int oldSize, int newSize);
    void setModel(QAbstractItemModel *model) override;
    void headerDataChanged(Qt::Orientation orientation, int logicalFirst, int logicalLast);
    void fixSectionPositions();

private:
    QVector<CustomHeaderView*> m_pTableFilterList;
};

#endif // CUSTOMHORIZONTALHEADERVIEW_H
