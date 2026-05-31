#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

namespace config {
    enum class PickMode {
        ALL_RANDOM = 1,
        ONE_BY_ONE = 2
    };

    class ConfigManager {
    private:
        PickMode mode;
        size_t nameCount;
        
    public:
        ConfigManager();
        bool loadFromFile(const std::string& filename);
        bool saveToFile(const std::string& filename) const;
        void setMode(PickMode newMode);
        PickMode getMode() const;
        void setNameCount(size_t count);
        size_t getNameCount() const;
        std::string getModeDescription() const;
    };
}

#endif // CONFIG_MANAGER_H