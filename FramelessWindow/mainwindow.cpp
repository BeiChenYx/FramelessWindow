#include <QHBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CustomModelView/customlineeditedelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->initUi();
    this->initConnetion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi()
{
    // 初始侧边导航为100px, 导航和内容的比例为 1 : 9
    QList<int> splitterList{100, this->width() - 108};
    ui->splitter->setSizes(splitterList);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 9);

    ui->buttonGroup_left_nav->setId(ui->toolButton_base, 0);
    ui->buttonGroup_left_nav->setId(ui->toolButton_table, 1);
    ui->buttonGroup_left_nav->setId(ui->toolButton_list, 2);
    ui->buttonGroup_left_nav->setId(ui->toolButton_tree, 3);
    ui->buttonGroup_left_nav->setId(ui->toolButton_custom, 4);

    ui->listView->setModel(&m_model);
    ui->treeView->setModel(&m_model);

    m_pCustomTableView = new CustomTableView(this);
    QHBoxLayout *pHLayout = new QHBoxLayout();
    pHLayout->addWidget(m_pCustomTableView);
    pHLayout->setMargin(9);
    ui->page_table->setLayout(pHLayout);
    m_pCustomTableView->setModel(&m_model);
    m_pCustomTableView->setColumnWidth(1, 150);

    // 初始化菜单
    m_pToListView = new QAction(tr("转到ListView"), this);
    m_pToTreeView = new QAction(tr("转到TreeView"), this);
    m_pToTableView = new QAction(tr("转到TableView"), this);
    m_pViewMenu = new QMenu(this);
    m_pViewMenu->addAction(m_pToListView);
    m_pViewMenu->addAction(m_pToTreeView);
    m_pViewMenu->addAction(m_pToTableView);
    m_pCustomTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listView->setContextMenuPolicy(Qt::CustomContextMenu);
}

void MainWindow::initConnetion()
{
    connect(ui->buttonGroup_left_nav, SIGNAL(buttonClicked(QAbstractButton *)),
                    this, SLOT(on_buttonClickedLeftNav(QAbstractButton *)));

    connect(m_pCustomTableView, &CustomTableView::customContextMenuRequested, this, [this](const QPoint &){
        m_pToTableView->setEnabled(false);
        m_pToListView->setEnabled(true);
        m_pToTreeView->setEnabled(true);
        m_pViewMenu->exec(QCursor::pos());
    });
    connect(ui->listView, &QListView::customContextMenuRequested, this, [this](const QPoint &){
        m_pToTableView->setEnabled(true);
        m_pToListView->setEnabled(false);
        m_pToTreeView->setEnabled(true);
        m_pViewMenu->exec(QCursor::pos());
    });
    connect(ui->treeView, &QTreeView::customContextMenuRequested, this, [this](const QPoint &){
        m_pToTableView->setEnabled(true);
        m_pToListView->setEnabled(true);
        m_pToTreeView->setEnabled(false);
        m_pViewMenu->exec(QCursor::pos());
    });
    auto whoView = [this]() -> QString{
        auto view = qApp->focusWidget();
        if(ui->listView == dynamic_cast<QListView*>(view)){
            return "QListView";
        }else if(ui->treeView == dynamic_cast<QTreeView*>(view)){
            return "QTreeView";
        }else if(m_pCustomTableView == dynamic_cast<CustomTableView*>(view)){
            return "CustomTableView";
        }else{ return ""; }
    };
    connect(m_pToListView, &QAction::triggered, this, [this, whoView](){
        auto who = whoView();
        qDebug() << who;
        ui->toolButton_list->click();
        if(who == "CustomTableView"){
            int row = m_pCustomTableView->currentIndex().row();
            auto index = ui->listView->model()->index(row, 0);
            ui->listView->setCurrentIndex(index);
        }else if(who == "QTreeView"){
            int row = ui->treeView->currentIndex().row();
            auto index = ui->listView->model()->index(row, 0);
            ui->listView->setCurrentIndex(index);
        }
    });
    connect(m_pToTreeView, &QAction::triggered, this, [this, whoView](){
        auto who = whoView();
        qDebug() << who;
        ui->toolButton_tree->click();
        if(who == "CustomTableView"){
            int row = m_pCustomTableView->currentIndex().row();
            auto index = ui->treeView->model()->index(row, 0);
            ui->treeView->setCurrentIndex(index);
        }else if(who == "QListView"){
            int row = ui->listView->currentIndex().row();
            auto index = ui->treeView->model()->index(row, 0);
            ui->treeView->setCurrentIndex(index);
        }
    });
    connect(m_pToTableView, &QAction::triggered, this, [this, whoView](){
        auto who = whoView();
        qDebug() << who;
        ui->toolButton_table->click();
        if(who == "QTreeView"){
            int row = ui->treeView->currentIndex().row();
            auto index = m_pCustomTableView->model()->index(row, 0);
            m_pCustomTableView->setCurrentIndex(index);
        }else if(who == "QListView"){
            int row = ui->listView->currentIndex().row();
            auto index = m_pCustomTableView->model()->index(row, 0);
            m_pCustomTableView->setCurrentIndex(index);
        }
    });
}

void MainWindow::on_buttonClickedLeftNav(QAbstractButton *btn)
{
    ui->stackedWidget->setCurrentIndex(ui->buttonGroup_left_nav->id(btn));
    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);
}
