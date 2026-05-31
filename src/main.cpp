#include <iostream>
#include <string>
#include "config/config_manager.h"
#include "data/name_list.h"
#include "core/randomizer.h"
#include "ui/console_ui.h"
#include "ui/tui.h"
#include "utils/platform.h"
#include "i18n/localizer.h"

class Application {
private:
    config::ConfigManager configManager;
    data::NameList nameList;
    core::Randomizer* randomizer;
    ui::TUI tui;
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
        if (nameList.isEmpty()) {
            tui.showEmptyScreen();
            return;
        }

        setupRandomizer();
        bool hideNext = false;

        while (true) {
            if (!randomizer->hasNext()) {
                tui.showDoneScreen();
                break;
            }

            size_t index = randomizer->getNextIndex();
            const std::string& name = nameList.getNameAt(index);
            size_t current = nameList.getCount() - randomizer->getRemainingCount() - 1;
            size_t total = nameList.getCount();
            hideNext = false;

            while (true) {
                tui.showPickScreen(name, current, total,
                                  configManager.getModeDescription(), hideNext);

                ui::TUIAction action = tui.getAction();
                if (action == ui::TUIAction::NEXT) {
                    break;
                } else if (action == ui::TUIAction::QUIT) {
                    utils::Platform::setRawMode(false);
                    return;
                } else if (action == ui::TUIAction::HIDE) {
                    hideNext = !hideNext;
                } else if (action == ui::TUIAction::MODE_ALL) {
                    configManager.setMode(config::PickMode::ALL_RANDOM);
                    delete randomizer;
                    randomizer = new core::Randomizer(core::RandomMode::ALL_RANDOM);
                    randomizer->initialize(nameList.getNames());
                    hideNext = false;
                    continue;
                } else if (action == ui::TUIAction::MODE_ONE) {
                    configManager.setMode(config::PickMode::ONE_BY_ONE);
                    delete randomizer;
                    randomizer = new core::Randomizer(core::RandomMode::ONE_BY_ONE);
                    randomizer->initialize(nameList.getNames());
                    hideNext = false;
                    continue;
                }
            }
        }

        utils::Platform::setRawMode(false);
    }

private:
    void setupRandomizer() {
        delete randomizer;
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
        randomizer = new core::Randomizer(coreMode);
        randomizer->initialize(nameList.getNames());
    }
};

int main(int argc, char* argv[]) {
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
