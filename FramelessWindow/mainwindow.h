#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QStandardItemModel>
#include <QMenu>
#include <QAction>
#include <QListView>
#include <QTreeView>
#include "CustomModelView/custommodel.h"
#include "CustomModelView/customlistview.h"
#include "CustomModelView/customtableview.h"
#include "CustomModelView/customtreeview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initConnetion();
    void initUi();

    /* 添加自定义的页面到QStackWidget中，并添加左导航栏按钮
     * @name: 左导航按钮需要显示的名字
     * @widget: 需要插入的页面
     */
    void addNavStackWidget(QString name, QString text, QWidget *widget);
    // 向导航栏中添加水平分割线
    void addNavHLine();

private slots:
    void on_buttonClickedLeftNav(QAbstractButton *btn);

private:
    Ui::MainWindow *ui;
    QButtonGroup *m_pBtnGroupLeftNav;
    QVector<QToolButton*> m_pTBtnLeftNavVector;

    CustomModel m_model;
    CustomTableView *m_pCustomTableView;
    QListView *m_pListView;
    QTreeView *m_pTreeView;

    // 菜单跳转
    QAction *m_pToListView;
    QAction *m_pToTreeView;
    QAction *m_pToTableView;
    QMenu *m_pViewMenu;
    QMap<QAction*, QString> m_viewBtnMap;
};

#endif // MAINWINDOW_H
