#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTranslator>
#include <vector>
#include <string>

namespace Ui {
class mainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onNextButtonClicked();
    void onTimerTimeout();
    void onExitActionTriggered();
    void onHideNextActionTriggered();
    void onAllRandomActionTriggered();
    void onOneByOneActionTriggered();
    void onRestartActionTriggered();
    void onLanguageEnglish();
    void onLanguageChinese();

private:
    void loadNamesFromFile();
    void updateUI();
    void initializeRandomizer();
    void updateProgress();
    void updateTime();
    void retranslateStrings();
    void switchLanguage(const QString& langCode);

    Ui::mainWindow *ui;
    QTranslator* translator;
    std::vector<std::string> names;
    std::vector<size_t> randomIndices;
    size_t currentIndex;
    bool hideNextPerson;
    bool allDone;
    int pickMode;
    QTimer *timer;
};

#endif
