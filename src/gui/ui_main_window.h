#ifndef UI_MAIN_WINDOW_H
#define UI_MAIN_WINDOW_H

#include <QtCore/QVariant>
#include <QApplication>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QTextBrowser>
#include <QWidget>
#include <QtGui/QAction>

QT_BEGIN_NAMESPACE

class Ui_mainWindow
{
public:
    QWidget *centralwidget;
    QTextBrowser *titleShower;
    QProgressBar *progressShower;
    QTextBrowser *BrowserName;
    QPushButton *ButtonNext;
    QTextBrowser *textBrowser;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_chmode;
    QStatusBar *statusbar;
    QAction *actionExit;
    QAction *action_hidenp;
    QAction *action_m_all;
    QAction *action_per;

    void setupUi(QMainWindow *mainWindow)
    {
        if (mainWindow->objectName().isEmpty())
            mainWindow->setObjectName(QString::fromUtf8("mainWindow"));
        mainWindow->resize(728, 463);
        
        // Create actions first
        actionExit = new QAction(mainWindow);
        actionExit->setObjectName(QString::fromUtf8("actionExit"));
        
        action_hidenp = new QAction(mainWindow);
        action_hidenp->setObjectName(QString::fromUtf8("action_hidenp"));
        
        action_m_all = new QAction(mainWindow);
        action_m_all->setObjectName(QString::fromUtf8("action_m_all"));
        
        action_per = new QAction(mainWindow);
        action_per->setObjectName(QString::fromUtf8("action_per"));
        
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        titleShower = new QTextBrowser(centralwidget);
        titleShower->setObjectName(QString::fromUtf8("titleShower"));
        titleShower->setGeometry(QRect(0, 0, 131, 31));
        QFont font1;
        font1.setPointSize(12);
        titleShower->setFont(font1);
        titleShower->setMarkdown(QString::fromUtf8("随机人员选择器\n\n"));
        progressShower = new QProgressBar(centralwidget);
        progressShower->setObjectName(QString::fromUtf8("progressShower"));
        progressShower->setGeometry(QRect(20, 330, 311, 23));
        progressShower->setValue(24);
        BrowserName = new QTextBrowser(centralwidget);
        BrowserName->setObjectName(QString::fromUtf8("BrowserName"));
        BrowserName->setGeometry(QRect(0, 40, 611, 271));
        QFont font2;
        font2.setPointSize(50);
        BrowserName->setFont(font2);
        ButtonNext = new QPushButton(centralwidget);
        ButtonNext->setObjectName(QString::fromUtf8("ButtonNext"));
        ButtonNext->setGeometry(QRect(380, 320, 181, 71));
        QFont font3;
        font3.setPointSize(20);
        ButtonNext->setFont(font3);
        ButtonNext->setText(QString::fromUtf8("下一个"));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(40, 380, 71, 16));
        QFont font4;
        font4.setPointSize(1);
        textBrowser->setFont(font4);
        textBrowser->setMarkdown(QString::fromUtf8("Jianyin Li,使用  Qt 框架\n\n"));
        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 728, 22));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setTitle(QString::fromUtf8("随机人员选择器"));
        menu_chmode = new QMenu(menu);
        menu_chmode->setObjectName(QString::fromUtf8("menu_chmode"));
        menu_chmode->setTitle(QString::fromUtf8("更改模式"));
        mainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(mainWindow);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        mainWindow->setStatusBar(statusbar);

        menubar->addAction(menu->menuAction());
        menu->addAction(actionExit);
        menu->addSeparator();
        menu->addAction(action_hidenp);
        menu->addSeparator();
        menu->addAction(menu_chmode->menuAction());
        menu_chmode->addAction(action_m_all);
        menu_chmode->addAction(action_per);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QCoreApplication::translate("mainWindow", "随机人员选择器 - 主窗口", nullptr));
        actionExit->setText(QCoreApplication::translate("mainWindow", "Exit", nullptr));
        action_hidenp->setText(QCoreApplication::translate("mainWindow", "隐藏下一位", nullptr));
        action_m_all->setText(QCoreApplication::translate("mainWindow", "全部覆盖的随机", nullptr));
        action_per->setText(QCoreApplication::translate("mainWindow", "逐个随机", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H