#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <QFileInfo>
#include "main_window.h"
#include "../config/config_manager.h"
#include "../i18n/localizer.h"
#include <fstream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("RandomNamePicker");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Jianyin Li");

    // Load config for language preference
    config::ConfigManager config;
    config.loadFromFile("data/config.conf");
    std::string langStr = config.getLanguage();

    // Parse --lang argument (overrides config)
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--lang" && i + 1 < argc) {
            langStr = argv[++i];
        }
    }

    i18n::Language lang = i18n::Localizer::parseLanguage(langStr);
    i18n::Localizer::setLanguage(lang);

    // Load Qt translator from embedded resources
    QTranslator translator;
    if (lang == i18n::Language::ZH_CN) {
        QFileInfo qmInfo(":/i18n/RandomNamePicker_zh_CN.qm");
        if (qmInfo.exists() && translator.load(":/i18n/RandomNamePicker_zh_CN.qm")) {
            app.installTranslator(&translator);
        }
    }

    MainWindow window;
    window.show();

    return app.exec();
}
