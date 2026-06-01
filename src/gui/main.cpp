#include <QApplication>
#include "main_window.h"
#include "../core/config_manager.h"
#include "../i18n/localizer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setApplicationName("RandomNamePicker");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("Jianyin Li");

    config::ConfigManager config;
    config.loadFromFile("data/config.conf");
    std::string langStr = config.getLanguage();

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--lang" && i + 1 < argc) {
            langStr = argv[++i];
        }
    }

    i18n::Localizer::setLanguage(langStr);

    MainWindow window;
    window.show();

    return app.exec();
}
