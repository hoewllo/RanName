#include "console_ui.h"
#include "../utils/platform.h"
#include "../i18n/localizer.h"
#include <iostream>
#include <string>

namespace ui {
    void ConsoleUI::showTitle(const std::string& title) {
        utils::Platform::clearScreen();
        utils::Platform::setConsoleTitle(title);
        std::cout << "+--------------------------------+" << std::endl;
        std::cout << "|          " << title << "         |" << std::endl;
        std::cout << "+--------------------------------+" << std::endl;
    }

    void ConsoleUI::showMainMenu() {
        showTitle(i18n::Localizer::get(i18n::ID::TITLE_MAIN));
    }

    void ConsoleUI::showConfigurationMenu() {
        showTitle(i18n::Localizer::get(i18n::ID::TITLE_CONFIG));
    }

    void ConsoleUI::showRandomPickingHeader() {
        showTitle(i18n::Localizer::get(i18n::ID::TITLE_PICKING));
    }

    void ConsoleUI::showName(const std::string& name, size_t remaining) {
        std::cout << i18n::Localizer::get(i18n::ID::NAME_LABEL) << name << std::endl;
        std::cout << i18n::Localizer::get(i18n::ID::REMAINING_LABEL) << remaining << i18n::Localizer::get(i18n::ID::PERSON_UNIT) << std::endl;
    }

    void ConsoleUI::showMessage(const std::string& message) {
        std::cout << message << std::endl;
    }

    void ConsoleUI::showProgress(const std::string& message, int dots) {
        std::cout << message << std::endl;
        for (int x = 0; x < dots; x++) {
            std::cout << ".";
            utils::Platform::sleep(100);
        }
        std::cout << std::endl;
    }

    int ConsoleUI::getUserChoice(int min, int max) {
        std::string input;
        while (true) {
            std::getline(std::cin, input);
            if (input.size() == 1 && input[0] >= '0' + min && input[0] <= '0' + max) {
                return input[0] - '0';
            }
            std::cout << i18n::Localizer::get(i18n::ID::INVALID_INPUT) << std::endl;
        }
    }

    std::string ConsoleUI::getUserInput() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    void ConsoleUI::waitForEnter() {
        std::cout << i18n::Localizer::get(i18n::ID::PRESS_ENTER);
        getUserInput();
    }

    bool ConsoleUI::askYesNo(const std::string& question) {
        std::cout << question << "(Y/n)" << std::endl;
        std::string input = getUserInput();
        return input != "n" && input != "N";
    }
}
