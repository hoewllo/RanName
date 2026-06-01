#ifndef TUI_H
#define TUI_H

#include <string>
#include <ctime>

namespace ui {

enum class TUIAction {
    NEXT,
    QUIT,
    HIDE,
    MODE_ALL,
    MODE_ONE,
    LANG,
    RESTART,
    SETUP,
    NONE
};

class TUI {
public:
    TUI();
    ~TUI();

    void showPickScreen(const std::string& name, size_t current, size_t total,
                        const std::string& modeDesc, bool hideNext);
    void showDoneScreen();
    void showEmptyScreen();

    TUIAction getAction();

private:
    void ansiMove(int row, int col);
    void ansiClearLine();
    void ansiClearScreen();

    void drawFrame();
    void drawHeader(const std::string& modeDesc);
    void drawName(const std::string& name, bool hideNext);
    void drawProgress(size_t current, size_t total);
    void drawStatus(const std::string& msg);
    void drawPrompt();

    TUIAction parseCommand(const std::string& cmd);
    std::string centerText(const std::string& text, int w);
    std::string currentTimeStr();

    int width;
    int height;
    std::time_t lastTimeUpdate;
    std::string cachedTimeStr;
    bool drawn;
    std::string lastStatus;
    int frameBottomRow;
};

}

#endif
