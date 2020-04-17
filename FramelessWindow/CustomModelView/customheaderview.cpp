#include "customheaderview.h"
#include "ui_customheaderview.h"
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QDebug>


FilterDialog::FilterDialog(QWidget *parent) : QWidget(parent)
{
    this->setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    auto hLayout = new QHBoxLayout();
    m_pLineEdit = new QLineEdit();
    m_pBtn = new QPushButton();
    m_pBtn->setText(tr("确定"));
    hLayout->addWidget(m_pLineEdit);
    hLayout->addWidget(m_pBtn);
    this->setLayout(hLayout);
    this->setWindowModality(Qt::ApplicationModal);
    connect(m_pBtn, &QPushButton::clicked, this, [this](){
        emit filter(this->filterMsg());
        this->close();
    });
}


CustomHeaderView::CustomHeaderView(int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomHeaderView),
    m_index(index),
    m_pFilterDialog(new FilterDialog)
{
    ui->setupUi(this);
    ui->toolButton_filter->setVisible(true);
    ui->toolButton_sortUp->setVisible(false);
    ui->toolButton_sortDown->setVisible(false);
    this->setObjectName("CustomHeaderView");
    m_pFilterDialog->hide();
    connect(ui->toolButton_sortUp, &QToolButton::clicked, this, [this](){
        ui->buttonGroup->setExclusive(true);
        ui->toolButton_filter->setChecked(false);
        ui->toolButton_sortUp->setChecked(true);
        ui->toolButton_sortDown->setChecked(false);
        this->sortedUp(m_index);
    });
    connect(ui->toolButton_sortDown, &QToolButton::clicked, this, [this](){
        ui->buttonGroup->setExclusive(true);
        ui->toolButton_filter->setChecked(false);
        ui->toolButton_sortUp->setChecked(false);
        ui->toolButton_sortDown->setChecked(true);
        this->sortedDown(m_index);
    });

    connect(ui->toolButton_filter, &QToolButton::clicked, this, [this](){
        ui->buttonGroup->setExclusive(true);
        ui->toolButton_filter->setChecked(true);
        ui->toolButton_sortUp->setChecked(false);
        ui->toolButton_sortDown->setChecked(false);
        auto zero = this->mapToGlobal(QPoint(ui->label_title->pos().x(), ui->label_title->pos().y()));
        m_pFilterDialog->move(zero.x(), zero.y() + this->height());
        m_pFilterDialog->resize(this->size() + QSize(5, 0));
        m_pFilterDialog->show();
    });
    connect(m_pFilterDialog, &FilterDialog::filter, this, [this](QString msg){
        this->filter(m_index, msg);
    });
    ui->widget_header->installEventFilter(this);
    ui->label_title->installEventFilter(this);
    ui->toolButton_filter->installEventFilter(this);
    ui->toolButton_sortUp->installEventFilter(this);
    ui->toolButton_sortDown->installEventFilter(this);
}

CustomHeaderView::~CustomHeaderView()
{
    delete ui;
    if(m_pFilterDialog != nullptr){
        delete m_pFilterDialog;
        m_pFilterDialog = nullptr;
    }
}

void CustomHeaderView::setTitle(QString text)
{
    ui->label_title->setText(text);
}
QString CustomHeaderView::title()
{
    return ui->label_title->text();
}

void CustomHeaderView::setAlignment(Qt::Alignment align)
{
    ui->label_title->setAlignment(align);
}

void CustomHeaderView::sortUpVisible(bool status)
{
    ui->toolButton_sortUp->setVisible(status);
}

void CustomHeaderView::sortDownVisible(bool status)
{
    ui->toolButton_sortDown->setVisible(status);
}

void CustomHeaderView::filterVisible(bool status)
{
    ui->toolButton_filter->setVisible(status);
}

void CustomHeaderView::clearStatus()
{
    ui->buttonGroup->setExclusive(false);
    ui->toolButton_filter->setChecked(false);
    ui->toolButton_sortUp->setChecked(false);
    ui->toolButton_sortDown->setChecked(false);
}
void CustomHeaderView::clearFilterStatus()
{
    ui->buttonGroup->setExclusive(false);
    ui->toolButton_filter->setChecked(false);
}

QString CustomHeaderView::getFilterMsg()
{
    return m_pFilterDialog->filterMsg();
}

bool CustomHeaderView::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget_header || obj == ui->label_title || obj == ui->toolButton_filter
            || obj == ui->toolButton_sortUp || obj == ui->toolButton_sortDown){
            setCursor(Qt::ArrowCursor);
    }
    if(event->type() == QEvent::Leave){
//        ui->toolButton_filter->setVisible(false);
        ui->toolButton_sortUp->setVisible(false);
        ui->toolButton_sortDown->setVisible(false);
    }else if(event->type() == QEvent::Enter){
//        ui->toolButton_filter->setVisible(true);
        ui->toolButton_sortUp->setVisible(true);
        ui->toolButton_sortDown->setVisible(true);
    }
    return QWidget::eventFilter(obj, event);
}


