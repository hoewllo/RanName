#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include "main_window.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 设置应用程序信息
    app.setApplicationName("随机人员选择器");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Jianyin Li");
    
    // 创建并显示主窗口
    MainWindow window;
    window.show();
    
    return app.exec();
}