#include "config_manager.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace config {
    ConfigManager::ConfigManager() : mode(PickMode::ALL_RANDOM), nameCount(0) {}

    bool ConfigManager::loadFromFile(const std::string& filename) {
        std::ifstream configFile(filename);
        if (!configFile.is_open()) {
            std::cerr << "无法打开配置文件: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(configFile, line)) {
            if (line.find("[mode]=") != std::string::npos) {
                int modeValue = std::stoi(line.substr(line.find("=") + 1));
                mode = static_cast<PickMode>(modeValue);
            } else if (line.find("[name_count]=") != std::string::npos) {
                nameCount = std::stoul(line.substr(line.find("=") + 1));
            }
        }
        configFile.close();
        return true;
    }

    bool ConfigManager::saveToFile(const std::string& filename) const {
        std::ofstream configFile(filename);
        if (!configFile.is_open()) {
            std::cerr << "无法打开配置文件进行写入: " << filename << std::endl;
            return false;
        }

        configFile << "[mode]= " << static_cast<int>(mode) << std::endl;
        configFile << "[name_count]= " << nameCount << std::endl;
        configFile.close();
        return true;
    }

    void ConfigManager::setMode(PickMode newMode) {
        mode = newMode;
    }

    PickMode ConfigManager::getMode() const {
        return mode;
    }

    void ConfigManager::setNameCount(size_t count) {
        nameCount = count;
    }

    size_t ConfigManager::getNameCount() const {
        return nameCount;
    }

    std::string ConfigManager::getModeDescription() const {
        switch (mode) {
            case PickMode::ALL_RANDOM:
                return "全局随机";
            case PickMode::ONE_BY_ONE:
                return "逐个随机";
            default:
                return "全局随机";
        }
    }
}