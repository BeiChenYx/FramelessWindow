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
#include "basicwidget.h"

namespace Ui {
class MainWindow;
}

class VersionInfoWidget : public QWidget
{
    Q_OBJECT
public:
    explicit VersionInfoWidget(QString version="V1.0.0", QWidget *parent = nullptr)
        : QWidget(parent){
        this->setObjectName("customStatusBar");
        QHBoxLayout *hLayout = new QHBoxLayout;
        QLabel *label = new QLabel(version, this);
        hLayout->addWidget(label);
        hLayout->setContentsMargins(0, 0, 0, 0);
        this->setLayout(hLayout);
        this->setStyleSheet("#customStatusBar QLabel{color: #FFFFFF;}");
    }
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    // 框架   --start
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initConnetion();
    void initUi();

    /* 添加自定义的页面到QStackWidget中，并添加左导航栏按钮
     * @name: 左导航按钮需要显示的名字
     * @widget: 需要插入的页面
     */
    void addNavStackWidget(QString objectName, QString text, QWidget *widget, QIcon ico=QIcon());
    // 向导航栏中添加水平分割线
    void addNavHLine();
    // 框架   --end

private slots:
    // 框架   --start
    void on_buttonClickedLeftNav(QAbstractButton *btn);
    // 框架   --end

private:
    // 框架   --start
    Ui::MainWindow *ui;
    QButtonGroup *m_pBtnGroupLeftNav;
    QVector<QToolButton*> m_pTBtnLeftNavVector;
    VersionInfoWidget *m_pVersionWidget;
    // 框架   --end

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

    BasicWidget *m_pBasicWidget;
};

#endif // MAINWINDOW_H
