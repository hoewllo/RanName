#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "config/config_manager.h"
#include "data/name_list.h"
#include "ui/console_ui.h"
#include "utils/platform.h"

void showNameList(const data::NameList& list) {
    std::cout << "\n当前名单 (" << list.getCount() << " 人):" << std::endl;
    for (size_t i = 0; i < list.getCount(); i++) {
        std::cout << (i + 1) << ". " << list.getNameAt(i) << std::endl;
    }
}

void editNameList(data::NameList& list) {
    while (true) {
        ui::ConsoleUI::showConfigurationMenu();
        showNameList(list);
        std::cout << "\n1. 添加姓名" << std::endl;
        std::cout << "2. 清空名单" << std::endl;
        std::cout << "3. 保存并返回" << std::endl;
        std::cout << "请选择: ";

        int choice = ui::ConsoleUI::getUserChoice(1, 3);
        switch (choice) {
            case 1: {
                std::cout << "请输入姓名: ";
                std::string name = ui::ConsoleUI::getUserInput();
                if (!name.empty()) {
                    list.addName(name);
                }
                break;
            }
            case 2:
                list.clear();
                std::cout << "名单已清空。" << std::endl;
                break;
            case 3:
                return;
        }
    }
}

int main() {
    utils::Platform::setUTF8Encoding();

    config::ConfigManager config;
    data::NameList nameList;

    config.loadFromFile("data/config.conf");
    nameList.loadFromFile("data/namelist.txt");

    while (true) {
        ui::ConsoleUI::showConfigurationMenu();
        std::cout << "1. 编辑名单 (" << nameList.getCount() << " 人)" << std::endl;
        std::cout << "2. 设置模式 (当前: " << config.getModeDescription() << ")" << std::endl;
        std::cout << "3. 保存并退出" << std::endl;
        std::cout << "请选择: ";

        int choice = ui::ConsoleUI::getUserChoice(1, 3);
        switch (choice) {
            case 1:
                editNameList(nameList);
                break;
            case 2: {
                std::cout << "选择模式:" << std::endl;
                std::cout << "1. 全部覆盖的随机" << std::endl;
                std::cout << "2. 逐个随机" << std::endl;
                int mode = ui::ConsoleUI::getUserChoice(1, 2);
                config.setMode(static_cast<config::PickMode>(mode));
                break;
            }
            case 3: {
                config.setNameCount(nameList.getCount());
                config.saveToFile("data/config.conf");

                std::ofstream file("data/namelist.txt");
                if (file.is_open()) {
                    for (size_t i = 0; i < nameList.getCount(); i++) {
                        file << nameList.getNameAt(i) << std::endl;
                    }
                    file.close();
                }

                std::cout << "设置已保存。" << std::endl;
                return 0;
            }
        }
    }

    return 0;
}
