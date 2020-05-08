#ifndef BASICWIDGET_H
#define BASICWIDGET_H

#include <QWidget>

namespace Ui {
class BasicWidget;
}

class BasicWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BasicWidget(QWidget *parent = nullptr);
    ~BasicWidget();

private:
    Ui::BasicWidget *ui;
};

#endif // BASICWIDGET_H
