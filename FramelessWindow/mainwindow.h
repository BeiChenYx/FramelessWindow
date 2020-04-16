#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <QStandardItemModel>
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
    CustomTableView *m_pCustomTableView;
    CustomModel m_model;

    QStandardItemModel *m_pStandardModel;
};

#endif // MAINWINDOW_H
