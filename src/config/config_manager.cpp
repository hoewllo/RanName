#include "config_manager.h"
#include "../i18n/localizer.h"
#include <fstream>
#include <iostream>
#include <sstream>

namespace config {
    ConfigManager::ConfigManager() : mode(PickMode::ALL_RANDOM), nameCount(0), language("en_US") {}

    bool ConfigManager::loadFromFile(const std::string& filename) {
        std::ifstream configFile(filename);
        if (!configFile.is_open()) {
            std::cerr << i18n::Localizer::get(i18n::ID::UNABLE_OPEN_CONFIG) << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(configFile, line)) {
            if (line.find("[mode]=") != std::string::npos) {
                int modeValue = std::stoi(line.substr(line.find("=") + 1));
                mode = static_cast<PickMode>(modeValue);
            } else if (line.find("[name_count]=") != std::string::npos) {
                nameCount = std::stoul(line.substr(line.find("=") + 1));
            } else if (line.find("[language]=") != std::string::npos) {
                language = line.substr(line.find("=") + 1);
                // Trim whitespace
                if (!language.empty() && language[0] == ' ') {
                    language = language.substr(1);
                }
            }
        }
        configFile.close();
        return true;
    }

    bool ConfigManager::saveToFile(const std::string& filename) const {
        std::ofstream configFile(filename);
        if (!configFile.is_open()) {
            std::cerr << i18n::Localizer::get(i18n::ID::UNABLE_WRITE_CONFIG) << filename << std::endl;
            return false;
        }

        configFile << "[mode]= " << static_cast<int>(mode) << std::endl;
        configFile << "[name_count]= " << nameCount << std::endl;
        configFile << "[language]= " << language << std::endl;
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
                return i18n::Localizer::get(i18n::ID::MODE_ALL_RANDOM);
            case PickMode::ONE_BY_ONE:
                return i18n::Localizer::get(i18n::ID::MODE_ONE_BY_ONE);
            default:
                return i18n::Localizer::get(i18n::ID::MODE_ALL_RANDOM);
        }
    }

    void ConfigManager::setLanguage(const std::string& lang) {
        language = lang;
    }

    std::string ConfigManager::getLanguage() const {
        return language;
    }
}
