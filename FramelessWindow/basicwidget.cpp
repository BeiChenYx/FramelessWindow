#include "basicwidget.h"
#include "ui_basicwidget.h"

BasicWidget::BasicWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BasicWidget)
{
    ui->setupUi(this);
}

BasicWidget::~BasicWidget()
{
    delete ui;
}
