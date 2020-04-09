#ifndef CUSTOMTABLE_H
#define CUSTOMTABLE_H

#include <QObject>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QModelIndex>
#include <QSortFilterProxyModel>
#include <QLabel>
#include <QToolButton>
#include <QHBoxLayout>
#include <QVBoxLayout>

// 1. 先实现正常的视图/模型显示
// 2. 再实现定制表头
// 3. 处理选择
// 4. 实现右键菜单
// 5. 在不同的View之间交互跳转, QListView/QTreeView/QTableView

class TableHeaderFilter;

/* 自定义TableView, 用来在表头进行过滤筛选及排序等操作 */
class CustomTableView : public QWidget
{
    Q_OBJECT
public:
    explicit CustomTableView(QTableView *table = nullptr, QWidget *parent = nullptr);
    void setTableView(QTableView *table, QAbstractItemModel *model){
        m_pTableView = table;
        m_pTableView->setModel(model);
        this->initUi();
    }

signals:

public slots:

private:
    void initUi();

private:
    QHeaderView *m_pHeaderView;
    QTableView *m_pTableView;
    QSortFilterProxyModel *m_pSortFilterModel;
    QVector<TableHeaderFilter *> m_pTableFilterList;
};

class TableHeaderFilter : public QWidget
{
    Q_OBJECT
public:
    explicit TableHeaderFilter(int index, QWidget *parent = nullptr);
    virtual ~TableHeaderFilter(){}
signals:
    void sortedUp();
    void sortedDown();
public slots:
    void setText(QString text){
        this->m_pTitle->setText(text);
    }

private:
    QLabel *m_pTitle;
    QToolButton *m_pFilter;
    QToolButton *m_pSorterUp;
    QToolButton *m_pSorterDown;
    int m_index = 0;
};

#endif // CUSTOMTABLE_H
