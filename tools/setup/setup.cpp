#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "core/config_manager.h"
#include "core/name_list.h"
#include "ui/setup_tui.h"
#include "utils/platform.h"
#include "i18n/localizer.h"

static void showNameList(const data::NameList& list) {
    std::cout << "\n" << i18n::Localizer::get(i18n::ID::CURRENT_LIST)
              << list.getCount() << i18n::Localizer::get(i18n::ID::NAMES_COUNT) << std::endl;
    for (size_t i = 0; i < list.getCount(); i++) {
        std::cout << (i + 1) << ". " << list.getNameAt(i) << std::endl;
    }
}

static void saveAll(config::ConfigManager& config, data::NameList& nameList) {
    config.setNameCount(nameList.getCount());
    config.setLanguage(i18n::Localizer::languageToString(i18n::Localizer::getLanguage()));
    config.saveToFile("data/config.conf");

    std::ofstream file("data/namelist.txt");
    if (file.is_open()) {
        for (size_t i = 0; i < nameList.getCount(); i++) {
            file << nameList.getNameAt(i) << std::endl;
        }
        file.close();
    }
}

static void doEditList(data::NameList& nameList, ui::SetupTUI& tui) {
    while (true) {
        std::vector<std::string> names;
        for (size_t i = 0; i < nameList.getCount(); i++) {
            names.push_back(nameList.getNameAt(i));
        }
        tui.showListScreen(names);
        std::string cmd = tui.getCommand();

        std::string lower = cmd;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower == "back" || lower == "b" || lower == "return"
            || lower == "exit" || lower == "quit" || lower == "q") {
            return;
        }
        if (lower == "clear") {
            nameList.clear();
            continue;
        }
        if (lower == "list" || lower == "names" || lower == "show") {
            continue;
        }
        if (lower == "help" || lower == "?") {
            continue;
        }
        if (cmd.size() > 4 && lower.substr(0, 4) == "add ") {
            std::string name = cmd.substr(4);
            if (!name.empty()) {
                nameList.addName(name);
            }
            continue;
        }

        // Unknown command — show list screen again (handled by loop)
    }
}

int main(int argc, char* argv[]) {
    bool langSetByArg = false;
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--lang" && i + 1 < argc) {
            std::string langStr = argv[++i];
            if (langStr == "zh-CN" || langStr == "zh_CN" || langStr == "zh") {
                i18n::Localizer::setLanguage("zh_CN");
            } else {
                i18n::Localizer::setLanguage("en_US");
            }
            langSetByArg = true;
        }
    }

    utils::Platform::setUTF8Encoding();

    config::ConfigManager config;
    data::NameList nameList;
    config.loadFromFile("data/config.conf");
    if (!langSetByArg) {
        i18n::Localizer::setLanguage(config.getLanguage());
    }
    nameList.loadFromFile("data/namelist.txt");

    ui::SetupTUI tui;

    while (true) {
        tui.showMainScreen(config.getModeDescription(), nameList.getCount());
        std::string cmd = tui.getCommand();

        std::string lower = cmd;
        std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

        if (lower == "exit" || lower == "quit" || lower == "q") {
            break;
        }
        if (lower == "save") {
            saveAll(config, nameList);
            tui.showMainScreen(config.getModeDescription(), nameList.getCount(),
                               i18n::Localizer::get(i18n::ID::SETTINGS_SAVED));
            tui.getCommand();
            break;
        }
        if (lower == "list") {
            doEditList(nameList, tui);
            continue;
        }
        if (lower == "mode" || lower == "mode all" || lower == "mode_all"
            || lower == "mode all random") {
            config.setMode(config::PickMode::ALL_RANDOM);
            continue;
        }
        if (lower == "mode one" || lower == "mode_one"
            || lower == "mode one by one") {
            config.setMode(config::PickMode::ONE_BY_ONE);
            continue;
        }
        if (lower == "help" || lower == "?") {
            continue;
        }
        // Unknown command — just redraw
    }

    return 0;
}
