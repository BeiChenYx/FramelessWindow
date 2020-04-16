#include <QLineEdit>
#include "customlineeditedelegate.h"

CustomLineEditeDelegate::CustomLineEditeDelegate(QObject *parent) : QItemDelegate(parent)
{

}

QWidget *CustomLineEditeDelegate::createEditor(QWidget *parent,
                      const QStyleOptionViewItem &option,
                      const QModelIndex &index) const
{
    return new QLineEdit(parent);
}

void CustomLineEditeDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();
    auto lineEdit = static_cast<QLineEdit*>(editor);
    lineEdit->setText(value);
}
void CustomLineEditeDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    auto lineEdit = static_cast<QLineEdit*>(editor);
    model->setData(index, lineEdit->text());
}

void CustomLineEditeDelegate::updateEditorGeometry(QWidget *editor,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const
{
    Q_UNUSED(index);
    editor->setGeometry(option.rect);
}
