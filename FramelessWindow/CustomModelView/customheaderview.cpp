#include "customheaderview.h"
#include "ui_customheaderview.h"

CustomHeaderView::CustomHeaderView(int index, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomHeaderView),
    m_index(index)
{
    ui->setupUi(this);
    this->setAutoFillBackground(true);
    connect(ui->toolButton_sortUp, &QToolButton::clicked, this, [this](){
        this->sortedUp(m_index);
    });
    connect(ui->toolButton_sortDown, &QToolButton::clicked, this, [this](){
        this->sortedDown(m_index);
    });
    connect(ui->toolButton_filter, &QToolButton::clicked, this, [this](){
        this->filter(m_index);
    });
}

CustomHeaderView::~CustomHeaderView()
{
    delete ui;
}

void CustomHeaderView::setTitle(QString text)
{
    ui->label_title->setText(text);
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
