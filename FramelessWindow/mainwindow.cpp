#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    ui->tableView->setModel(&m_model);
    ui->treeView->setModel(&m_model);
    ui->tableView->initUi();
}

void MainWindow::initConnetion()
{
    connect(ui->buttonGroup_left_nav, SIGNAL(buttonClicked(QAbstractButton *)),
                    this, SLOT(on_buttonClickedLeftNav(QAbstractButton *)));
}

void MainWindow::on_buttonClickedLeftNav(QAbstractButton *btn)
{
    ui->stackedWidget->setCurrentIndex(ui->buttonGroup_left_nav->id(btn));
    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);
}
