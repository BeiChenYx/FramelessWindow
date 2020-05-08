#include <QHBoxLayout>
#include <QFrame>
#include <QHBoxLayout>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "CustomModelView/customlineeditedelegate.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pBtnGroupLeftNav(new QButtonGroup(this)),
    m_pVersionWidget(new VersionInfoWidget("版本号: V1.0.0")),
    m_pCustomTableView(new CustomTableView()),
    m_pListView(new QListView()),
    m_pTreeView(new QTreeView()),
    m_pBasicWidget(new BasicWidget(this))
{
    // 框架   --start
    ui->setupUi(this);
    this->initUi();
    this->initConnetion();
    // 框架   --end
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initUi()
{
    // 框架   --start
    // 初始侧边导航为100px, 导航和内容的比例为 1 : 9
    QList<int> splitterList{100, this->width() - 108};
    ui->splitter->setSizes(splitterList);
    ui->splitter->setStretchFactor(0, 1);
    ui->splitter->setStretchFactor(1, 9);
    ui->statusbar->addPermanentWidget(m_pVersionWidget);
    for(auto pComboBox: findChildren<QComboBox*>()){
        pComboBox->setView(new QListView());
    }
    // 框架   --end

    // 测试 --start
    this->addNavStackWidget("", tr("基本组件"), m_pBasicWidget);
    this->addNavHLine();

    auto tableWidget = new QWidget(this);
    tableWidget->setLayout(new QHBoxLayout);
    tableWidget->layout()->addWidget(m_pCustomTableView);
    tableWidget->setObjectName("tableWidget");
    this->addNavStackWidget("tableBtn", "tableView", tableWidget);

    auto listWidget = new QWidget(this);
    listWidget->setLayout(new QHBoxLayout);
    listWidget->layout()->addWidget(m_pListView);
    listWidget->setObjectName("listWidget");
    this->addNavStackWidget("listBtn", "listView", listWidget);

    auto treeWidget = new QWidget(this);
    treeWidget->setLayout(new QHBoxLayout);
    treeWidget->layout()->addWidget(m_pTreeView);
    treeWidget->setObjectName("treeWidget");
    this->addNavStackWidget("treeBtn", "treeView", treeWidget);

    this->addNavHLine();
    this->addNavStackWidget("", tr("自绘"), new QWidget(this), QIcon(":/images/filter.png"));

    m_pListView->setModel(&m_model);
    m_pTreeView->setModel(&m_model);

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
    m_viewBtnMap[m_pToTableView] = "tableBtn";
    m_viewBtnMap[m_pToListView] = "listBtn";
    m_viewBtnMap[m_pToTreeView] = "treeBtn";
    m_pCustomTableView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pTreeView->setContextMenuPolicy(Qt::CustomContextMenu);
    m_pListView->setContextMenuPolicy(Qt::CustomContextMenu);
    // 测试 --end
}

void MainWindow::initConnetion()
{
    // 框架   --start
    connect(m_pBtnGroupLeftNav, SIGNAL(buttonClicked(QAbstractButton *)),
                    this, SLOT(on_buttonClickedLeftNav(QAbstractButton *)));
    // 框架   --end

    // 测试 --start
    connect(m_pCustomTableView, &CustomTableView::customContextMenuRequested, this, [this](const QPoint &){
        m_pToTableView->setEnabled(false);
        m_pToListView->setEnabled(true);
        m_pToTreeView->setEnabled(true);
        m_pViewMenu->exec(QCursor::pos());
    });
    connect(m_pListView, &QListView::customContextMenuRequested, this, [this](const QPoint &){
        m_pToTableView->setEnabled(true);
        m_pToListView->setEnabled(false);
        m_pToTreeView->setEnabled(true);
        m_pViewMenu->exec(QCursor::pos());
    });
    connect(m_pTreeView, &QTreeView::customContextMenuRequested, this, [this](const QPoint &){
        m_pToTableView->setEnabled(true);
        m_pToListView->setEnabled(true);
        m_pToTreeView->setEnabled(false);
        m_pViewMenu->exec(QCursor::pos());
    });
    connect(m_pToListView, &QAction::triggered, this, [this](){
        auto view = qApp->focusWidget();
        auto who = m_viewBtnMap.value(m_pToListView, "");
        auto btn = findChild<QToolButton*>(who);
        btn->click();
        if(view == m_pCustomTableView){
            int row = m_pCustomTableView->currentIndex().row();
            auto index = m_pListView->model()->index(row, 0);
            m_pListView->setCurrentIndex(index);
        }else if(view == m_pTreeView){
            int row = m_pTreeView->currentIndex().row();
            auto index = m_pListView->model()->index(row, 0);
            m_pListView->setCurrentIndex(index);
        }
    });
    connect(m_pToTreeView, &QAction::triggered, this, [this](){
        auto view = qApp->focusWidget();
        auto who = m_viewBtnMap.value(m_pToTreeView, "");
        auto btn = findChild<QToolButton*>(who);
        btn->click();
        if(view == m_pCustomTableView){
            int row = m_pCustomTableView->currentIndex().row();
            auto index = m_pTreeView->model()->index(row, 0);
            m_pTreeView->setCurrentIndex(index);
        }else if(view == m_pListView){
            int row = m_pListView->currentIndex().row();
            auto index = m_pTreeView->model()->index(row, 0);
            m_pTreeView->setCurrentIndex(index);
        }
    });
    connect(m_pToTableView, &QAction::triggered, this, [this](){
        auto view = qApp->focusWidget();
        auto who = m_viewBtnMap.value(m_pToTableView, "");
        auto btn = findChild<QToolButton*>(who);
        btn->click();
        if(view == m_pTreeView){
            int row = m_pTreeView->currentIndex().row();
            auto index = m_pCustomTableView->model()->index(row, 0);
            m_pCustomTableView->setCurrentIndex(index);
        }else if(view == m_pListView){
            int row = m_pListView->currentIndex().row();
            auto index = m_pCustomTableView->model()->index(row, 0);
            m_pCustomTableView->setCurrentIndex(index);
        }
    });
    // 测试 --end
}

void MainWindow::on_buttonClickedLeftNav(QAbstractButton *btn)
{
    ui->stackedWidget->setCurrentIndex(m_pBtnGroupLeftNav->id(btn));
    QToolButton *obj = qobject_cast<QToolButton*>(btn);
    obj->setChecked(true);
}

void MainWindow::addNavStackWidget(QString objectName, QString text, QWidget *widget, QIcon ico)
{
    int btnId = m_pTBtnLeftNavVector.length();
    auto pTBtn = new QToolButton(this);
    m_pTBtnLeftNavVector.append(pTBtn);
    pTBtn->setText(text);
    pTBtn->setCheckable(true);
    pTBtn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    if(!objectName.trimmed().isEmpty()){
        pTBtn->setObjectName(objectName);
    }
    if(!ico.isNull()){
        pTBtn->setIcon(ico);
        pTBtn->setIconSize(QSize(16, 16));
        pTBtn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }else{
        pTBtn->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
    m_pBtnGroupLeftNav->addButton(pTBtn, btnId);
    ui->verticalLayout_left_nav->addWidget(pTBtn);
    ui->stackedWidget->addWidget(widget);
}

void MainWindow::addNavHLine()
{
    QFrame *line = new QFrame(this);
    line->setGeometry(QRect(40, 180, 400, 3));
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->raise();
    line->setObjectName("HLine");
    ui->verticalLayout_left_nav->addWidget(line);
}
