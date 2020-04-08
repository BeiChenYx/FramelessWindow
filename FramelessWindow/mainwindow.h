#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
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

private slots:
    void on_buttonClickedLeftNav(QAbstractButton *btn);

private:
    Ui::MainWindow *ui;

    CustomModel m_model;
};

#endif // MAINWINDOW_H
