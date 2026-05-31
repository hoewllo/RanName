#include "console_ui.h"
#include "../utils/platform.h"
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
        showTitle("随机点名程序");
    }

    void ConsoleUI::showConfigurationMenu() {
        showTitle("随机点名配置");
    }

    void ConsoleUI::showRandomPickingHeader() {
        showTitle("随机点名");
    }

    void ConsoleUI::showName(const std::string& name, size_t remaining) {
        std::cout << "姓名：" << name << std::endl;
        std::cout << "剩余：" << remaining << "人" << std::endl;
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
            std::cout << "无效输入，请重新选择。" << std::endl;
        }
    }

    std::string ConsoleUI::getUserInput() {
        std::string input;
        std::getline(std::cin, input);
        return input;
    }

    void ConsoleUI::waitForEnter() {
        std::cout << "按回车继续...";
        getUserInput();
    }

    bool ConsoleUI::askYesNo(const std::string& question) {
        std::cout << question << "(Y/n)" << std::endl;
        std::string input = getUserInput();
        return input != "n" && input != "N";
    }
}