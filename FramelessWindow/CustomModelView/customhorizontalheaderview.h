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
    ~CustomHorizontalHeaderView();
    void fixComboPositions();

signals:
    void sortedUp(int index);
    void sortedDown(int index);
    void filter(int index, QString msg);

public slots:

protected:
    void showEvent(QShowEvent *e);
    void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;
    void resizeEvent(QResizeEvent *event);
    void handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex);
    void handleSectionResized(int i, int oldSize, int newSize);

private:
    QVector<CustomHeaderView*> m_pTableFilterList;
};

#endif // CUSTOMHORIZONTALHEADERVIEW_H
