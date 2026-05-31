#include <iostream>
#include <string>
#include "config/config_manager.h"
#include "data/name_list.h"
#include "core/randomizer.h"
#include "ui/console_ui.h"
#include "utils/platform.h"

class Application {
private:
    config::ConfigManager configManager;
    data::NameList nameList;
    core::Randomizer* randomizer;
    
public:
    Application() : randomizer(nullptr) {
        initialize();
    }
    
    ~Application() {
        delete randomizer;
    }
    
    void initialize() {
        // Setup platform
        utils::Platform::setUTF8Encoding();
        utils::Platform::createDirectory("data");
        
        // Load configuration and data
        configManager.loadFromFile("data/config.conf");
        nameList.loadFromFile("data/namelist.txt");
        
        // Update name count in config if needed
        if (configManager.getNameCount() != nameList.getCount()) {
            configManager.setNameCount(nameList.getCount());
            configManager.saveToFile("data/config.conf");
        }
    }
    
    void run() {
        while (true) {
            showMainScreen();
            
            if (nameList.isEmpty()) {
                ui::ConsoleUI::showMessage("名单为空，请先配置名单。");
                ui::ConsoleUI::waitForEnter();
                continue;
            }
            
            performRandomPicking();
            
            if (!ui::ConsoleUI::askYesNo("继续使用吗？")) {
                break;
            }
        }
    }
    
private:
    void showMainScreen() {
        ui::ConsoleUI::showMainMenu();
        std::cout << "最后等待一下，您的理想课堂中的随机点名程序正在进行使用前的最后准备..." << std::endl;
        std::cout << "当前点名模式：" << configManager.getModeDescription() << std::endl;
        std::cout << "名单人数：" << nameList.getCount() << std::endl;
        std::cout << "您可以通过setup.exe进行设置。" << std::endl;
        
        ui::ConsoleUI::showProgress("请稍后");
    }
    
    void performRandomPicking() {
        // Convert config mode to core mode
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
        
        ui::ConsoleUI::showMessage("结束");
        utils::Platform::sleep(1000);
    }
};

int main() {
    Application app;
    app.run();
    return 0;
}