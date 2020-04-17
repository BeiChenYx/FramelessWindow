#include <QDesktopWidget>
#include <QTranslator>
#include <QGraphicsDropShadowEffect>
#include <QScreen>
#include "framelesswidget.h"
#include "ui_framelesswidget.h"
#include <QDebug>

FramelessWidget::FramelessWidget(bool isMax, bool isFull, QWidget *parent) :
    FramelessWidget(parent)
{
    m_isMax = isMax;
    m_isFull = isMax ? isFull : false;
    ui->toolButton_max->setVisible(m_isMax);
}

FramelessWidget::FramelessWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FramelessWidget)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowSystemMenuHint);
    // append minimize button flag in case of windows,
    // for correct windows native handling of minimize function
#if defined(Q_OS_WIN)
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint);
#endif
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    ui->toolButton_restore->setVisible(false);
    ui->toolButton_full->setVisible(false);
    ui->toolButton_full_exit->setVisible(false);

    // shadow under window title text
    QGraphicsDropShadowEffect *textShadow = new QGraphicsDropShadowEffect;
    textShadow->setBlurRadius(4.0);
    textShadow->setColor(QColor(0, 0, 0));
    textShadow->setOffset(0.0);
    ui->label_title->setGraphicsEffect(textShadow);
    this->styleWindow(true, true);
    QWidget::setWindowTitle(tr("FrameLessWidget"));

    QObject::connect(qApp, &QGuiApplication::applicationStateChanged, this,
                     &FramelessWidget::on_applicationStateChanged);
    setMouseTracking(true);
    this->setWindowIcon(QIcon(":/images/icon.png"));
    //  处理默认菜单的英文
//    QTranslator translator;
//    translator.load(QString(":/images/qt_zh_CN.qm"));
//    QApplication::installTranslator(&translator);
    QApplication::instance()->installEventFilter(this);
}

FramelessWidget::~FramelessWidget()
{
    delete ui;
}

void FramelessWidget::setWindowTitle(const QString &text)
{
    ui->label_title->setText(text);
    QWidget::setWindowTitle(text);
}
void FramelessWidget::setWindowIcon(const QIcon &ico)
{
    ui->label_icon->setPixmap(ico.pixmap(24, 24));
}
void FramelessWidget::setContent(QWidget *w)
{
    QHBoxLayout *pHlayout = new QHBoxLayout;
    pHlayout->addWidget(w);
    pHlayout->setMargin(0);
    ui->windowContent->setLayout(pHlayout);
}
void FramelessWidget::setLayout(QLayout *layout)
{
    QWidget *pWidget = new QWidget(this);
    pWidget->setLayout(layout);
    pWidget->setContentsMargins(0, 0, 0, 0);
    setContent(pWidget);
}

void FramelessWidget::on_applicationStateChanged(Qt::ApplicationState state)
{
    if (windowState().testFlag(Qt::WindowNoState)) {
        styleWindow(state == Qt::ApplicationActive, true);
    } else if (windowState().testFlag(Qt::WindowFullScreen)) {
        styleWindow(state == Qt::ApplicationActive, false);
    }
}


void FramelessWidget::on_toolButton_close_clicked() { this->close(); }

void FramelessWidget::on_toolButton_max_clicked()
{
    ui->toolButton_restore->setVisible(true);
    ui->toolButton_max->setVisible(false);
    if(m_isFull){
        ui->toolButton_full->setVisible(true);
    }
    this->setWindowState(Qt::WindowMaximized);
    this->showMaximized();
    this->styleWindow(true, false);
    this->update();
}

void FramelessWidget::on_toolButton_restore_clicked()
{
    ui->toolButton_restore->setVisible(false);
    ui->toolButton_max->setVisible(true);
    ui->toolButton_full->setVisible(false);
    setWindowState(Qt::WindowNoState);
    this->styleWindow(true, true);
    this->update();
}

void FramelessWidget::on_toolButton_min_clicked()
{
    this->showMinimized();
    this->update();
}

void FramelessWidget::on_toolButton_full_clicked()
{
    ui->toolButton_restore->setVisible(false);
    ui->toolButton_max->setVisible(false);
    ui->toolButton_full->setVisible(false);
    ui->toolButton_min->setVisible(false);
    ui->toolButton_close->setVisible(false);
    ui->toolButton_full_exit->setVisible(true);
    // 设置了Qt::WA_TranslucentBackground属性后无法直接调用showFullScreen接口实现全屏
    m_fullPoint = QPoint(this->geometry().x(), this->geometry().y());
    m_fullSize = QSize(this->geometry().width(), this->geometry().height());
    QDesktopWidget *deskdop = QApplication::desktop();
    this->move(0, 0);
    this->resize(deskdop->width(), deskdop->height());
    this->setWindowState(Qt::WindowFullScreen);
    this->update();
}

void FramelessWidget::on_toolButton_full_exit_clicked()
{
    ui->toolButton_restore->setVisible(true);
    ui->toolButton_max->setVisible(false);
    ui->toolButton_full->setVisible(true);
    ui->toolButton_min->setVisible(true);
    ui->toolButton_close->setVisible(true);
    ui->toolButton_full_exit->setVisible(false);
    this->move(m_fullPoint);
    this->resize(m_fullSize);
    this->setWindowState(Qt::WindowMaximized);
    this->update();
}

void FramelessWidget::styleWindow(bool bActive, bool bNoState)
{
    auto setShadow = [this](QColor color){
#if defined(Q_OS_WIN)
        this->layout()->setMargin(15);
#else
        this->layout()->setMargin(0);
#endif
        QGraphicsEffect *oldShadow = ui->windowFrame->graphicsEffect();
        if (oldShadow){delete oldShadow;}
        QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect;
        windowShadow->setBlurRadius(9.0);
        windowShadow->setColor(color);
        windowShadow->setOffset(0);
        ui->windowFrame->setGraphicsEffect(windowShadow);
    };
    auto clearShadow = [this](){
        this->layout()->setMargin(0);
        QGraphicsEffect *oldShadow = ui->windowFrame->graphicsEffect();
        if (oldShadow) delete oldShadow;
        ui->windowFrame->setGraphicsEffect(nullptr);
    };
    auto color =  palette().color(bActive ? QPalette::Highlight : QPalette::Shadow);
    if(bNoState){
        setShadow(color);
    }else{
        clearShadow();
    }
}

bool FramelessWidget::eventFilter(QObject *obj, QEvent *event) {
    // check mouse move event when mouse is moved on any object
    if (event->type() == QEvent::MouseMove) {
        if (isMaximized() || isFullScreen()) {
            return QWidget::eventFilter(obj, event);
        }
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
        if (pMouse) {
            checkBorderDragging(pMouse);
            if(obj == ui->windowTitlebar && m_bMousePressedTitle){
                move(m_wndPos + (pMouse->globalPos() - m_mousePos));
            }
        }
    }
    // press is triggered only on frame window
    else if (event->type() == QEvent::MouseButtonPress && obj == this) {
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
        if (pMouse) {
            mousePressEvent(pMouse);
        }
    } else if(event->type() == QEvent::MouseButtonPress && obj == ui->windowTitlebar){
        QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
        if (pMouse) {
            this->m_bMousePressedTitle = true;
            this->m_mousePos = pMouse->globalPos();
            this->m_wndPos = this->pos();
        }
    } else if (event->type() == QEvent::MouseButtonRelease && obj == this) {
        if (m_bMousePressed) {
            QMouseEvent *pMouse = dynamic_cast<QMouseEvent *>(event);
            if (pMouse) {
                mouseReleaseEvent(pMouse);
            }
        }
    } else if(event->type() == QEvent::MouseButtonRelease && obj == ui->windowTitlebar){
        m_bMousePressedTitle = false;
    } else if(event->type() == QEvent::MouseButtonDblClick && obj == ui->windowTitlebar){
        if(windowState().testFlag(Qt::WindowNoState) && m_isMax){
            this->on_toolButton_max_clicked();
        }else if(windowState().testFlag(Qt::WindowFullScreen)){
            this->on_toolButton_full_exit_clicked();
        }else if(windowState().testFlag(Qt::WindowMaximized)){
            this->on_toolButton_restore_clicked();
        }
    }

    return QWidget::eventFilter(obj, event);
}

// pos in global virtual desktop coordinates
bool FramelessWidget::leftBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    if (pos.x() >= rect.x() && pos.x() <= rect.x() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }
    return false;
}

bool FramelessWidget::rightBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    int tmp = rect.x() + rect.width();
    if (pos.x() <= tmp && pos.x() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}

bool FramelessWidget::topBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    if (pos.y() >= rect.y() && pos.y() <= rect.y() + CONST_DRAG_BORDER_SIZE) {
        return true;
    }
    return false;
}

bool FramelessWidget::bottomBorderHit(const QPoint &pos) {
    const QRect &rect = this->geometry();
    int tmp = rect.y() + rect.height();
    if (pos.y() <= tmp && pos.y() >= (tmp - CONST_DRAG_BORDER_SIZE)) {
        return true;
    }
    return false;
}

void FramelessWidget::mousePressEvent(QMouseEvent *event) {
    if (isMaximized()) { return; }

    m_bMousePressed = true;
    m_StartGeometry = this->geometry();

    QPoint globalMousePos = mapToGlobal(QPoint(event->x(), event->y()));

    if (leftBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
        m_bDragTop = true;
        m_bDragLeft = true;
        setCursor(Qt::SizeFDiagCursor);
    } else if (rightBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
        m_bDragRight = true;
        m_bDragTop = true;
        setCursor(Qt::SizeBDiagCursor);
    } else if (leftBorderHit(globalMousePos) && bottomBorderHit(globalMousePos)) {
        m_bDragLeft = true;
        m_bDragBottom = true;
        setCursor(Qt::SizeBDiagCursor);
    } else if(rightBorderHit(globalMousePos) && bottomBorderHit(globalMousePos)){
        m_bDragBottom = true;
        m_bDragRight = true;
        setCursor(Qt::SizeFDiagCursor);
    } else {
        if (topBorderHit(globalMousePos)) {
            m_bDragTop = true;
            setCursor(Qt::SizeVerCursor);
        } else if (leftBorderHit(globalMousePos)) {
            m_bDragLeft = true;
            setCursor(Qt::SizeHorCursor);
        } else if (rightBorderHit(globalMousePos)) {
            m_bDragRight = true;
            setCursor(Qt::SizeHorCursor);
        } else if (bottomBorderHit(globalMousePos)) {
            m_bDragBottom = true;
            setCursor(Qt::SizeVerCursor);
        }
    }
}

void FramelessWidget::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    if (isMaximized()) { return; }

    m_bMousePressed = false;
    bool bSwitchBackCursorNeeded = m_bDragTop || m_bDragLeft || m_bDragRight || m_bDragBottom;
    m_bDragTop = false;
    m_bDragLeft = false;
    m_bDragRight = false;
    m_bDragBottom = false;
    if (bSwitchBackCursorNeeded) { setCursor(Qt::ArrowCursor); }
}

void FramelessWidget::checkBorderDragging(QMouseEvent *event) {
    if (isMaximized()  || isFullScreen()) { return; }

    QPoint globalMousePos = event->globalPos();
    if (m_bMousePressed) {
        if (m_bDragTop && m_bDragRight) {
            int newHeight = m_StartGeometry.height() + m_StartGeometry.y() - globalMousePos.y();
            int newWidth = globalMousePos.x() - m_StartGeometry.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                setGeometry(m_StartGeometry.x(), globalMousePos.y(), newWidth, newHeight);
            }
        }else if (m_bDragTop && m_bDragLeft) {
            int newHeight = m_StartGeometry.height() + m_StartGeometry.y() - globalMousePos.y();
            int newWidth = m_StartGeometry.width() + m_StartGeometry.x() - globalMousePos.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                setGeometry(globalMousePos.x(), globalMousePos.y(), newWidth, newHeight);
            }
        }else if (m_bDragBottom && m_bDragLeft) {
            int newHeight = globalMousePos.y() - m_StartGeometry.y();
            int newWidth = m_StartGeometry.width() + m_StartGeometry.x() - globalMousePos.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                setGeometry(globalMousePos.x(), m_StartGeometry.y(), newWidth, newHeight);
            }
        } else if (m_bDragBottom && m_bDragRight) {
            int newHeight = globalMousePos.y() - m_StartGeometry.y();
            int newWidth = globalMousePos.x() - m_StartGeometry.x();
            if (newHeight > this->minimumHeight() && newWidth > this->minimumWidth()) {
                resize(newWidth, newHeight);
            }
        } else if (m_bDragTop) {
            int newHeight = m_StartGeometry.height() + m_StartGeometry.y() - globalMousePos.y();
            if (newHeight > this->minimumHeight()) {
                setGeometry(m_StartGeometry.x(), globalMousePos.y(), m_StartGeometry.width(),newHeight);
            }
        } else if (m_bDragLeft) {
            int newWidth = m_StartGeometry.width() + m_StartGeometry.x() - globalMousePos.x();
            if (newWidth > this->minimumWidth()) {
                setGeometry(globalMousePos.x(), m_StartGeometry.y(), newWidth, m_StartGeometry.height());
            }
        } else if (m_bDragRight) {
            int newWidth = globalMousePos.x() - m_StartGeometry.x();
            if (newWidth > this->minimumWidth()) {
                resize(newWidth, m_StartGeometry.height());
            }
        } else if (m_bDragBottom) {
            int newHeight = globalMousePos.y() - m_StartGeometry.y();
            if (newHeight > this->minimumHeight()) {
                resize(m_StartGeometry.width(), newHeight);
            }
        }
    } else {
        // no mouse pressed
        if (leftBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
            setCursor(Qt::SizeFDiagCursor);
        } else if (rightBorderHit(globalMousePos) && topBorderHit(globalMousePos)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if (leftBorderHit(globalMousePos) && bottomBorderHit(globalMousePos)) {
            setCursor(Qt::SizeBDiagCursor);
        } else if(rightBorderHit(globalMousePos) && bottomBorderHit(globalMousePos)){
            setCursor(Qt::SizeFDiagCursor);
        } else {
            if (topBorderHit(globalMousePos)) {
                setCursor(Qt::SizeVerCursor);
            } else if (leftBorderHit(globalMousePos)) {
                setCursor(Qt::SizeHorCursor);
            } else if (rightBorderHit(globalMousePos)) {
                setCursor(Qt::SizeHorCursor);
            } else if (bottomBorderHit(globalMousePos)) {
                setCursor(Qt::SizeVerCursor);
            } else {
                m_bDragTop = false;
                m_bDragLeft = false;
                m_bDragRight = false;
                m_bDragBottom = false;
                setCursor(Qt::ArrowCursor);
            }
        }
    }
}
