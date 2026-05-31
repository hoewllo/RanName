#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "config/config_manager.h"
#include "data/name_list.h"
#include "ui/console_ui.h"
#include "utils/platform.h"
#include "i18n/localizer.h"

void showNameList(const data::NameList& list) {
    std::cout << "\n" << i18n::Localizer::get(i18n::ID::CURRENT_LIST)
              << list.getCount() << i18n::Localizer::get(i18n::ID::NAMES_COUNT) << std::endl;
    for (size_t i = 0; i < list.getCount(); i++) {
        std::cout << (i + 1) << ". " << list.getNameAt(i) << std::endl;
    }
}

void editNameList(data::NameList& list) {
    while (true) {
        ui::ConsoleUI::showConfigurationMenu();
        showNameList(list);
        std::cout << "\n" << i18n::Localizer::get(i18n::ID::ADD_NAME) << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::CLEAR_LIST) << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::SAVE_RETURN) << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::PLEASE_CHOOSE);

        int choice = ui::ConsoleUI::getUserChoice(1, 3);
        switch (choice) {
            case 1: {
                std::cout << i18n::Localizer::get(i18n::ID::ENTER_NAME);
                std::string name = ui::ConsoleUI::getUserInput();
                if (!name.empty()) {
                    list.addName(name);
                }
                break;
            }
            case 2:
                list.clear();
                std::cout << i18n::Localizer::get(i18n::ID::LIST_CLEARED) << std::endl;
                break;
            case 3:
                return;
        }
    }
}

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

    utils::Platform::setUTF8Encoding();

    config::ConfigManager config;
    data::NameList nameList;

    config.loadFromFile("data/config.conf");
    if (!langSetByArg) {
        i18n::Localizer::setLanguage(
            i18n::Localizer::parseLanguage(config.getLanguage()));
    }

    nameList.loadFromFile("data/namelist.txt");

    while (true) {
        ui::ConsoleUI::showConfigurationMenu();
        std::cout << i18n::Localizer::get(i18n::ID::EDIT_LIST)
                  << nameList.getCount() << ")" << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::SET_MODE)
                  << config.getModeDescription() << ")" << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::SAVE_EXIT) << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::PLEASE_CHOOSE);

        int choice = ui::ConsoleUI::getUserChoice(1, 3);
        switch (choice) {
            case 1:
                editNameList(nameList);
                break;
            case 2: {
                std::cout << i18n::Localizer::get(i18n::ID::CHOOSE_MODE) << std::endl;
                std::cout << i18n::Localizer::get(i18n::ID::MODE_1_ALL) << std::endl;
                std::cout << i18n::Localizer::get(i18n::ID::MODE_2_ONE) << std::endl;
                int mode = ui::ConsoleUI::getUserChoice(1, 2);
                config.setMode(static_cast<config::PickMode>(mode));
                break;
            }
            case 3: {
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

                std::cout << i18n::Localizer::get(i18n::ID::SETTINGS_SAVED) << std::endl;
                return 0;
            }
        }
    }

    return 0;
}
