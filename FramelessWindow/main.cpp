#include "framelesswidget.h"
#include <QApplication>
#include <QMainWindow>
#include <QThread>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 显示最大化和全屏
    FramelessWidget w;
    // 隐藏最大化和全屏
//    FramelessWidget w(false, false);
    // 显示最大化，关闭全屏
//    FramelessWidget w(true, false);
    // 隐藏最大化, 显示全屏, 这种情况应该无法打开全屏
//    FramelessWidget w(false, true);
    w.show();
    return a.exec();
}
