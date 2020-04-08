#ifndef CUSTOMTABLE_H
#define CUSTOMTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QModelIndex>
#include <QToolButton>

// 1. 先实现正常的视图/模型显示
// 2. 再实现定制表头
// 3. 处理选择
// 4. 实现右键菜单
// 5. 在不同的View之间交互跳转, QListView/QTreeView/QTableView


/* 自定义TableView, 用来在表头进行过滤筛选及排序等操作 */
class CustomTableView : public QTableView
{
    Q_OBJECT
public:
    explicit CustomTableView(QWidget *parent = nullptr);
    void initUi();

signals:

public slots:

private:
    QToolButton *m_pTBtnSort;
    QHeaderView *m_pHeaderView;
};

#endif // CUSTOMTABLE_H
