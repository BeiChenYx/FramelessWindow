#ifndef CUSTOMTREEVIEW_H
#define CUSTOMTREEVIEW_H

#include <QObject>
#include <QWidget>

class CustomTreeView : public QObject
{
    Q_OBJECT
public:
    explicit CustomTreeView(QObject *parent = nullptr);

signals:

public slots:
};

#endif // CUSTOMTREEVIEW_H
