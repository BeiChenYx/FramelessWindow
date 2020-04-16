#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QLineEdit>

namespace Ui {
class CustomHeaderView;
}

class FilterDialog;

/*
 * 自定义横向表头内容框
 */
class CustomHeaderView : public QWidget
{
    Q_OBJECT

public:
    explicit CustomHeaderView(int index, QWidget *parent = nullptr);
    ~CustomHeaderView();

    void setTitle(QString text);
    QString title();
    void setAlignment(Qt::Alignment align);
    void sortUpVisible(bool status);
    void sortDownVisible(bool status);
    void filterVisible(bool status);
    void clearStatus();
    void clearFilterStatus();
    QString getFilterMsg();

signals:
    void sortedUp(int index);
    void sortedDown(int index);
    void filter(int index, QString msg);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);

private:
    Ui::CustomHeaderView *ui;
    // 索引
    int m_index = 0;

    FilterDialog *m_pFilterDialog;
};

class FilterDialog : public QWidget
{
    Q_OBJECT

public:
    FilterDialog(QWidget *parent = nullptr);
    QString filterMsg(){
        return this->m_pLineEdit->text().trimmed();
    }
signals:
    void filter(QString msg);

private:
    QPushButton *m_pBtn;
    QLineEdit *m_pLineEdit;
};

#endif // CUSTOMHEADERVIEW_H
