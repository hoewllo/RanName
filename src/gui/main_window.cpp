#include "main_window.h"
#include "ui_main_window.h"
#include "../config/config_manager.h"
#include "../data/name_list.h"
#include "../core/randomizer.h"
#include "../i18n/localizer.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QObject>
#include <fstream>
#include <random>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow),
    currentIndex(0),
    hideNextPerson(false),
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
    ui->action_hidenp->setText(tr("Hide Next Person"));
    ui->action_m_all->setText(tr("All Random"));
    ui->action_per->setText(tr("One by One"));
    ui->actionExit->setText(tr("Exit"));
}

void MainWindow::loadNamesFromFile()
{
    names.clear();

    std::ifstream file("data/namelist.txt");
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                names.push_back(line);
            }
        }
        file.close();
    } else {
        names = {"Error", "Sample1", "Sample2", "Sample3", "Sample4"};
    }

    if (names.empty()) {
        QMessageBox::warning(this, tr("Warning"), tr("The name list is empty. Please configure it first!"));
    }
}

void MainWindow::initializeRandomizer()
{
    randomIndices.clear();
    currentIndex = 0;

    for (size_t i = 0; i < names.size(); i++) {
        randomIndices.push_back(i);
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(randomIndices.begin(), randomIndices.end(), g);
}

void MainWindow::onNextButtonClicked()
{
    if (currentIndex < names.size() - 1) {
        currentIndex++;
        updateUI();
    } else {
        QMessageBox::information(this, tr("Done"), tr("All people have been called!"));
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

void MainWindow::onLanguageEnglish()
{
    i18n::Localizer::setLanguage(i18n::Language::EN_US);
}

void MainWindow::onLanguageChinese()
{
    i18n::Localizer::setLanguage(i18n::Language::ZH_CN);
}

void MainWindow::updateUI()
{
    if (names.empty() || randomIndices.empty()) {
        ui->BrowserName->setPlainText(tr("List is empty"));
        ui->progressShower->setValue(0);
        return;
    }

    if (currentIndex < randomIndices.size()) {
        QString currentName = QString::fromStdString(names[randomIndices[currentIndex]]);
        ui->BrowserName->setPlainText(currentName);
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
    ui->progressShower->setValue(progress);
}

void MainWindow::updateTime()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString timeString = currentTime.toString("HH:mm:ss");
    statusBar()->showMessage(tr("Current time: %1").arg(timeString));
}
