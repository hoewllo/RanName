#include <iostream>
#include <string>
#include "config/config_manager.h"
#include "data/name_list.h"
#include "core/randomizer.h"
#include "ui/console_ui.h"
#include "utils/platform.h"
#include "i18n/localizer.h"

class Application {
private:
    config::ConfigManager configManager;
    data::NameList nameList;
    core::Randomizer* randomizer;
    bool langSet;

public:
    Application(bool langSetByArg = false) : randomizer(nullptr), langSet(langSetByArg) {
        initialize();
    }

    ~Application() {
        delete randomizer;
    }

    void initialize() {
        utils::Platform::setUTF8Encoding();
        utils::Platform::createDirectory("data");

        configManager.loadFromFile("data/config.conf");

        // Only read language from config if --lang was not specified
        if (!langSet) {
            i18n::Localizer::setLanguage(
                i18n::Localizer::parseLanguage(configManager.getLanguage()));
        }

        nameList.loadFromFile("data/namelist.txt");

        if (configManager.getNameCount() != nameList.getCount()) {
            configManager.setNameCount(nameList.getCount());
            configManager.saveToFile("data/config.conf");
        }
    }

    void run() {
        while (true) {
            showMainScreen();

            if (nameList.isEmpty()) {
                ui::ConsoleUI::showMessage(i18n::Localizer::get(i18n::ID::LIST_EMPTY));
                ui::ConsoleUI::waitForEnter();
                continue;
            }

            performRandomPicking();

            if (!ui::ConsoleUI::askYesNo(i18n::Localizer::get(i18n::ID::CONTINUE_ASK))) {
                break;
            }
        }
    }

private:
    void showMainScreen() {
        ui::ConsoleUI::showMainMenu();
        std::cout << i18n::Localizer::get(i18n::ID::PREPARING) << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::CURRENT_MODE) << configManager.getModeDescription() << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::NAME_COUNT) << nameList.getCount() << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::SETUP_HINT) << std::endl;

        ui::ConsoleUI::showProgress(i18n::Localizer::get(i18n::ID::PLEASE_WAIT));
    }

    void performRandomPicking() {
        core::RandomMode coreMode;
        switch (configManager.getMode()) {
            case config::PickMode::ONE_BY_ONE:
                coreMode = core::RandomMode::ONE_BY_ONE;
                break;
            case config::PickMode::ALL_RANDOM:
            default:
                coreMode = core::RandomMode::ALL_RANDOM;
                break;
        }

        delete randomizer;
        randomizer = new core::Randomizer(coreMode);
        randomizer->initialize(nameList.getNames());

        while (randomizer->hasNext()) {
            ui::ConsoleUI::showRandomPickingHeader();
            size_t index = randomizer->getNextIndex();
            ui::ConsoleUI::showName(nameList.getNameAt(index), randomizer->getRemainingCount());
            ui::ConsoleUI::waitForEnter();
        }

        ui::ConsoleUI::showMessage(i18n::Localizer::get(i18n::ID::END));
        utils::Platform::sleep(1000);
    }
};

int main(int argc, char* argv[]) {
    // Parse --lang argument
    bool langSetByArg = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--lang" && i + 1 < argc) {
            i18n::Localizer::setLanguage(i18n::Localizer::parseLanguage(argv[++i]));
            langSetByArg = true;
        }
    }

    Application app(langSetByArg);
    app.run();
    return 0;
}
