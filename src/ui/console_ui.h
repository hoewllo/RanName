#ifndef CONSOLE_UI_H
#define CONSOLE_UI_H

#include <string>

namespace ui {
    class ConsoleUI {
    public:
        static void showTitle(const std::string& title);
        static void showMainMenu();
        static void showConfigurationMenu();
        static void showRandomPickingHeader();
        static void showName(const std::string& name, size_t remaining);
        static void showMessage(const std::string& message);
        static void showProgress(const std::string& message, int dots = 50);
        static int getUserChoice(int min, int max);
        static std::string getUserInput();
        static void waitForEnter();
        static bool askYesNo(const std::string& question);
    };
}

#endif // CONSOLE_UI_H