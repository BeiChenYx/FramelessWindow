#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>

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
};

#endif // MAINWINDOW_H
