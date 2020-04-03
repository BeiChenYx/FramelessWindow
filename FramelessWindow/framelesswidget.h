#ifndef FRAMELESSWIDGET_H
#define FRAMELESSWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class FramelessWidget;
}

class FramelessWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FramelessWidget(QWidget *parent = nullptr);
    explicit FramelessWidget(bool isMax, bool isFull=true, QWidget *parent = nullptr);
    virtual ~FramelessWidget();
    void setWindowTitle(const QString &text);
    void setWindowIcon(const QIcon &ico);
    void setContent(QWidget *w);

private slots:
    // 根据窗口状态改变窗口的阴影
    void on_applicationStateChanged(Qt::ApplicationState state);
    void on_toolButton_close_clicked();
    void on_toolButton_max_clicked();
    void on_toolButton_restore_clicked();
    void on_toolButton_min_clicked();
    // 全屏进入和退出处理，在最大化才会显示全屏按钮
    void on_toolButton_full_clicked();
    void on_toolButton_full_exit_clicked();
    // 处理双击事件, 在WindowDragger中发送 doubleCLicked信号
    void on_windowTitlebar_doubleClicked();
    // 绘制窗口阴影, bActive：窗口是否激活, bNoState: 正常状态
    void styleWindow(bool bActive, bool bNoState);
    bool leftBorderHit(const QPoint &pos);
    bool rightBorderHit(const QPoint &pos);
    bool topBorderHit(const QPoint &pos);
    bool bottomBorderHit(const QPoint &pos);

protected:
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void checkBorderDragging(QMouseEvent *event);

private:
    Ui::FramelessWidget *ui;
    QRect m_StartGeometry;
    const quint8 CONST_DRAG_BORDER_SIZE = 15;
    bool m_bMousePressed = false;
    bool m_bDragTop = false;
    bool m_bDragLeft = false;
    bool m_bDragRight = false;
    bool m_bDragBottom = false;

    // 全屏之前的窗口位置和尺寸
    QPoint m_fullPoint;
    QSize m_fullSize;

    // 是否显示全屏按钮和最大化按钮
    bool m_isFull = true;
    bool m_isMax = true;
};

#endif // FRAMELESSWIDGET_H
