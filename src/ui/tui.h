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
    void drawFrame();
    void drawHeader(const std::string& modeDesc);
    void drawNameArea(const std::string& name, bool hideNext);
    void drawProgress(size_t current, size_t total);
    void drawControls();
    void drawTime();
    void moveTo(int row, int col);
    void clearLine(int row);
    std::string centerText(const std::string& text, int width);
    std::string currentTimeStr();

    int width;
    int height;
    std::time_t lastTimeUpdate;
    std::string cachedTimeStr;
};

}

#endif
