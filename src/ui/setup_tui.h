#ifndef SETUP_TUI_H
#define SETUP_TUI_H

#include <string>
#include <vector>

namespace ui {

class SetupTUI {
public:
    SetupTUI();
    ~SetupTUI();

    void showMainScreen(const std::string& modeDesc, size_t nameCount,
                        const std::string& statusMsg = "");
    void showListScreen(const std::vector<std::string>& names,
                        const std::string& statusMsg = "");

    std::string getCommand();

private:
    void drawTopBorder();
    void drawBottomBorder(int row);
    void drawContentRow(int row, const std::string& text);
    void drawPrompt();
    void clearScreen();

    int width;
    int frameBottomRow;
};

}

#endif
