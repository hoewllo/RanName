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
    QMenu *menu_lang;
    QStatusBar *statusbar;
    QAction *actionExit;
    QAction *action_hidenp;
    QAction *action_m_all;
    QAction *action_per;
    QAction *action_restart;
    QAction *action_lang_en;
    QAction *action_lang_zh;

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

        action_restart = new QAction(mainWindow);
        action_restart->setObjectName(QString::fromUtf8("action_restart"));

        action_lang_en = new QAction(mainWindow);
        action_lang_en->setObjectName(QString::fromUtf8("action_lang_en"));

        action_lang_zh = new QAction(mainWindow);
        action_lang_zh->setObjectName(QString::fromUtf8("action_lang_zh"));
        
        centralwidget = new QWidget(mainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        titleShower = new QTextBrowser(centralwidget);
        titleShower->setObjectName(QString::fromUtf8("titleShower"));
        titleShower->setGeometry(QRect(0, 0, 131, 31));
        QFont font1;
        font1.setPointSize(12);
        titleShower->setFont(font1);
        titleShower->setMarkdown(QString::fromUtf8("Random Name Picker\n\n"));
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
        ButtonNext->setText(QString::fromUtf8("Next"));
        textBrowser = new QTextBrowser(centralwidget);
        textBrowser->setObjectName(QString::fromUtf8("textBrowser"));
        textBrowser->setGeometry(QRect(40, 380, 71, 16));
        QFont font4;
        font4.setPointSize(1);
        textBrowser->setFont(font4);
        textBrowser->setMarkdown(QString::fromUtf8("Jianyin Li, Powered by Qt Framework\n\n"));
        mainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(mainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 728, 22));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu->setTitle(QString::fromUtf8("Random Name Picker"));
        menu_chmode = new QMenu(menu);
        menu_chmode->setObjectName(QString::fromUtf8("menu_chmode"));
        menu_chmode->setTitle(QString::fromUtf8("Change Mode"));

        menu_lang = new QMenu(menu);
        menu_lang->setObjectName(QString::fromUtf8("menu_lang"));
        menu_lang->setTitle(QString::fromUtf8("Language"));

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
        menu->addSeparator();
        menu->addAction(action_restart);
        menu->addSeparator();
        menu->addAction(menu_lang->menuAction());
        menu_lang->addAction(action_lang_en);
        menu_lang->addAction(action_lang_zh);
        menu_chmode->addAction(action_m_all);
        menu_chmode->addAction(action_per);

        retranslateUi(mainWindow);

        QMetaObject::connectSlotsByName(mainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *mainWindow)
    {
        mainWindow->setWindowTitle(QCoreApplication::translate("mainWindow", "Random Name Picker - Main Window", nullptr));
        actionExit->setText(QCoreApplication::translate("mainWindow", "Exit", nullptr));
        action_hidenp->setText(QCoreApplication::translate("mainWindow", "Hide Next Person", nullptr));
        action_m_all->setText(QCoreApplication::translate("mainWindow", "All Random", nullptr));
        action_per->setText(QCoreApplication::translate("mainWindow", "One by One", nullptr));
        action_restart->setText(QCoreApplication::translate("mainWindow", "Restart", nullptr));
        menu_lang->setTitle(QCoreApplication::translate("mainWindow", "Language", nullptr));
        action_lang_en->setText(QCoreApplication::translate("mainWindow", "English", nullptr));
        action_lang_zh->setText(QCoreApplication::translate("mainWindow", "中文", nullptr));
    } // retranslateUi

};

namespace Ui {
    class mainWindow: public Ui_mainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAIN_WINDOW_H
