#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

#include <QWidget>

namespace Ui {
class CustomHeaderView;
}

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
    void setAlignment(Qt::Alignment align);
    void sortUpVisible(bool status);
    void sortDownVisible(bool status);
    void filterVisible(bool status);

signals:
    void sortedUp(int index);
    void sortedDown(int index);
    void filter(int index);

private:
    Ui::CustomHeaderView *ui;

    // 索引
    int m_index = 0;
};

#endif // CUSTOMHEADERVIEW_H
