#ifndef CUSTOMHORIZONTALHEADERVIEW_H
#define CUSTOMHORIZONTALHEADERVIEW_H

#include <QObject>
#include <QHeaderView>

#include "customheaderview.h"

class CustomHorizontalHeaderView : public QHeaderView
{
    Q_OBJECT
public:
    explicit CustomHorizontalHeaderView(QWidget *parent = nullptr);
    void fixComboPositions();

signals:
    void sortedUp(int index);
    void sortedDown(int index);
    void filter(int index, QString msg);

public slots:

protected:
    void showEvent(QShowEvent *e);
    void handleSectionMoved(int logical, int oldVisualIndex, int newVisualIndex);
    void handleSectionResized(int i, int oldSize, int newSize);

private:
    QVector<CustomHeaderView*> m_pTableFilterList;
};

#endif // CUSTOMHORIZONTALHEADERVIEW_H
