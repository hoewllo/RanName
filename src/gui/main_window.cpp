#include "main_window.h"
#include "ui_main_window.h"
#include "../config/config_manager.h"
#include "../data/name_list.h"
#include "../core/randomizer.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QMessageBox>
#include <QObject>  // 添加QObject头文件
#include <fstream>
#include <random>
#include <algorithm>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::mainWindow),
    currentIndex(0),
    hideNextPerson(false),
    pickMode(1) // 默认全部覆盖的随机
{
    ui->setupUi(this);
    
    // 初始化定时器
    timer = new QTimer(this);
    
    // 连接信号和槽 - 使用Qt5兼容语法
    QObject::connect(ui->ButtonNext, &QPushButton::clicked, this, &MainWindow::onNextButtonClicked);
    QObject::connect(timer, &QTimer::timeout, this, &MainWindow::onTimerTimeout);
    QObject::connect(ui->actionExit, &QAction::triggered, this, &MainWindow::onExitActionTriggered);
    QObject::connect(ui->action_hidenp, &QAction::triggered, this, &MainWindow::onHideNextActionTriggered);
    QObject::connect(ui->action_m_all, &QAction::triggered, this, &MainWindow::onAllRandomActionTriggered);
    QObject::connect(ui->action_per, &QAction::triggered, this, &MainWindow::onOneByOneActionTriggered);
    
    // 加载名单和配置
    loadNamesFromFile();
    initializeRandomizer();
    
    // 启动定时器
    timer->start(1000);
    
    // 更新界面
    updateUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadNamesFromFile()
{
    names.clear();
    
    // 从文件加载名单
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
        // 如果文件不存在，添加一些示例名字
        names = {"错误", "李四", "张三", "文件", "检查"};
    }
    
    if (names.empty()) {
        QMessageBox::warning(this, "警告", "名单为空，请先配置名单！");
    }
}

void MainWindow::initializeRandomizer()
{
    randomIndices.clear();
    currentIndex = 0;
    
    // 创建索引
    for (size_t i = 0; i < names.size(); i++) {
        randomIndices.push_back(i);
    }
    
    // 随机打乱
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
        QMessageBox::information(this, "完成", "所有人员已点名完毕！");
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

void MainWindow::updateUI()
{
    if (names.empty() || randomIndices.empty()) {
        ui->BrowserName->setPlainText("名单为空");
        ui->progressShower->setValue(0);
        return;
    }
    
    // 显示当前名字
    if (currentIndex < randomIndices.size()) {
        QString currentName = QString::fromStdString(names[randomIndices[currentIndex]]);
        ui->BrowserName->setPlainText(currentName);
    }
    
    // 更新进度
    updateProgress();
    
    // 更新时间
    updateTime();
    
    // 更新窗口标题显示当前模式
    QString modeText = (pickMode == 1) ? "全部覆盖的随机" : "逐个随机";
    setWindowTitle(QString("随机人员选择器 - %1").arg(modeText));
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
    // 可以在状态栏显示时间
    statusBar()->showMessage(QString("当前时间: %1").arg(timeString));
}