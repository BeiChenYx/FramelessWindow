#include <QApplication>
#include <QTranslator>
#include "FrameLessWidget/framelesswidget.h"
#include "mainwindow.h"
#include "CustomModelView/customheaderview.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //  处理默认菜单的英文
    QTranslator translator;
    translator.load(QString(":/images/qt_zh_CN.qm"));
    QApplication::installTranslator(&translator);

    // 显示最大化和全屏
    FramelessWidget w;
    // 隐藏最大化和全屏
//    FramelessWidget w(false, false);
    // 显示最大化，关闭全屏
//    FramelessWidget w(true, false);
    // 隐藏最大化, 显示全屏, 这种情况应该无法打开全屏
//    FramelessWidget w(false, true);
    auto pMain = new MainWindow(&w);
    w.setContent(pMain);
    w.show();

    return a.exec();
}
