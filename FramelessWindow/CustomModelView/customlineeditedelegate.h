#ifndef CUSTOMLINEEDITEDELEGATE_H
#define CUSTOMLINEEDITEDELEGATE_H

#include <QObject>
#include <QItemDelegate>

class CustomLineEditeDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    explicit CustomLineEditeDelegate(QObject *parent = nullptr);
    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

signals:

public slots:
};

#endif // CUSTOMLINEEDITEDELEGATE_H
