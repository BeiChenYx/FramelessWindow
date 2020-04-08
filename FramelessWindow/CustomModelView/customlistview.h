#ifndef CUSTOMLISTVIEW_H
#define CUSTOMLISTVIEW_H

#include <QObject>
#include <QWidget>

class CustomListView : public QObject
{
    Q_OBJECT
public:
    explicit CustomListView(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CUSTOMLISTVIEW_H
