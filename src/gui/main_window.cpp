#include "main_window.h"
#include "ui_main_window.h"
#include "../config/config_manager.h"
#include "../data/name_list.h"
#include "../core/randomizer.h"
#include "../i18n/localizer.h"
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QObject>
#include <QApplication>
#include <fstream>
#include <random>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow),
    translator(new QTranslator(this)),
    currentIndex(0),
    hideNextPerson(false),
    allDone(false),
    pickMode(1)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    QObject::connect(ui->ButtonNext, &QPushButton::clicked, this, &MainWindow::onNextButtonClicked);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    QObject::connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExitActionTriggered);
    QObject::connect(ui->action_hidenp, &QAction::triggered, this, &MainWindow::onHideNextActionTriggered);
    QObject::connect(ui->action_m_all, &QAction::triggered, this, &MainWindow::onAllRandomActionTriggered);
    QObject::connect(ui->action_per, &QAction::triggered, this, &MainWindow::onOneByOneActionTriggered);
    QObject::connect(ui->action_restart, &QAction::triggered, this, &MainWindow::onRestartActionTriggered);
    QObject::connect(ui->action_lang_en, &QAction::triggered, this, &MainWindow::onLanguageEnglish);
    QObject::connect(ui->action_lang_zh, &QAction::triggered, this, &MainWindow::onLanguageChinese);

    retranslateStrings();

    loadNamesFromFile();
    initializeRandomizer();

    timer->start(1000);

    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::retranslateStrings()
{
    ui->titleShower->setMarkdown(tr("Random Name Picker"));
    ui->ButtonNext->setText(tr("Next"));
    ui->textBrowser->setMarkdown(tr("Jianyin Li, Powered by Qt Framework"));
    setWindowTitle(tr("Random Name Picker - Main Window"));

    ui->menu->setTitle(tr("Random Name Picker"));
    ui->menu_chmode->setTitle(tr("Change Mode"));
    ui->menu_lang->setTitle(tr("Language"));
    ui->action_hidenp->setText(tr("Hide Next Person"));
    ui->action_m_all->setText(tr("All Random"));
    ui->action_per->setText(tr("One by One"));
    ui->action_restart->setText(tr("Restart"));
    ui->actionExit->setText(tr("Exit"));
    ui->action_lang_en->setText(tr("English"));
    ui->action_lang_zh->setText(tr("中文"));
}

void MainWindow::switchLanguage(const QString& langCode)
{
    QString qmPath = QString(":/i18n/RandomNamePicker_%1.qm").arg(langCode);
    QFileInfo qmInfo(qmPath);

    qApp->removeTranslator(translator);

    if (qmInfo.exists() && translator->load(qmPath)) {
        qApp->installTranslator(translator);
    }

    i18n::Language lang = i18n::Localizer::parseLanguage(langCode.toStdString());
    i18n::Localizer::setLanguage(lang);

    ui->retranslateUi(this);
    retranslateStrings();
    updateUI();

    config::ConfigManager cfg;
    cfg.loadFromFile("data/config.conf");
    cfg.setLanguage(i18n::Localizer::languageToString(lang));
    cfg.saveToFile("data/config.conf");
}

void MainWindow::onLanguageEnglish()
{
    switchLanguage("en_US");
}

void MainWindow::onLanguageChinese()
{
    switchLanguage("zh_CN");
}

void MainWindow::loadNamesFromFile()
{
    data::NameList dataList;
    dataList.loadFromFile("data/namelist.txt");

    names.clear();
    for (size_t i = 0; i < dataList.getCount(); i++) {
        names.push_back(dataList.getNameAt(i));
    }

    if (names.empty()) {
        QMessageBox::warning(this, tr("Warning"), tr("The name list is empty. Please configure it first!"));
    }
}

void MainWindow::initializeRandomizer()
{
    randomIndices.clear();
    currentIndex = 0;
    allDone = false;

    for (size_t i = 0; i < names.size(); i++) {
        randomIndices.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(randomIndices.begin(), randomIndices.end(), g);
}

void MainWindow::onNextButtonClicked()
{
    if (allDone) {
        onRestartActionTriggered();
        return;
    }

    if (currentIndex < names.size() - 1) {
        currentIndex++;
        updateUI();
    } else {
        allDone = true;
        updateUI();
    }
}

void MainWindow::onTimerTimeout()
{
    updateTime();
}

void MainWindow::onExitActionTriggered()
{
    close();
}

void MainWindow::onHideNextActionTriggered()
{
    hideNextPerson = !hideNextPerson;
    updateUI();
}

void MainWindow::onAllRandomActionTriggered()
{
    pickMode = 1;
    initializeRandomizer();
    updateUI();
}

void MainWindow::onOneByOneActionTriggered()
{
    pickMode = 2;
    initializeRandomizer();
    updateUI();
}

void MainWindow::onRestartActionTriggered()
{
    initializeRandomizer();
    updateUI();
}

void MainWindow::updateUI()
{
    if (names.empty() || randomIndices.empty()) {
        ui->BrowserName->setPlainText(tr("List is empty"));
        ui->progressShower->setValue(0);
        return;
    }

    if (allDone) {
        ui->BrowserName->setPlainText(tr("All done! Click Next to restart."));
        ui->progressShower->setValue(100);
    } else if (currentIndex < randomIndices.size()) {
        if (hideNextPerson) {
            ui->BrowserName->setPlainText(tr("*** Hidden ***"));
        } else {
            QString currentName = QString::fromStdString(names[randomIndices[currentIndex]]);
            ui->BrowserName->setPlainText(currentName);
        }
    }

    updateProgress();
    updateTime();

    QString modeText = (pickMode == 1) ? tr("All Random") : tr("One by One");
    setWindowTitle(tr("Random Name Picker - %1").arg(modeText));
}

void MainWindow::updateProgress()
{
    if (names.empty()) {
        ui->progressShower->setValue(0);
        return;
    }

    int progress = (currentIndex + 1) * 100 / names.size();
    if (progress > 100) progress = 100;
    ui->progressShower->setValue(progress);
}

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("HH:mm:ss");
    statusBar()->showMessage(tr("Current time: %1").arg(timeString));
}
