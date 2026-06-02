#include "tui.h"
#include "../utils/platform.h"
#include "../i18n/localizer.h"
#include <iostream>
#include <sstream>
#include <algorithm>

namespace ui {

static const char* TOP_LEFT     = "\xe2\x95\x94";
static const char* TOP_RIGHT    = "\xe2\x95\x97";
static const char* BOT_LEFT     = "\xe2\x95\x9a";
static const char* BOT_RIGHT    = "\xe2\x95\x9d";
static const char* HORIZ        = "\xe2\x95\x90";
static const char* VERT         = "\xe2\x95\x91";
static const char* TOP_MID      = "\xe2\x95\xa6";
static const char* BOT_MID      = "\xe2\x95\xa9";
static const char* LEFT_MID     = "\xe2\x95\xa0";
static const char* RIGHT_MID    = "\xe2\x95\xa3";
static const char* PROGRESS_ON  = "\xe2\x96\x88";
static const char* PROGRESS_OFF = "\xe2\x96\x91";

TUI::TUI()
    : width(78)
    , height(24)
    , lastTimeUpdate(0)
    , drawn(false)
    , frameBottomRow(8)
{
    int tw = utils::Platform::getTerminalWidth();
    if (tw > 0) width = tw - 2;
    if (width < 40) width = 40;
    if (width > 120) width = 120;
}

TUI::~TUI() {
    utils::Platform::setRawMode(false);
    std::cout << "\033[0m\033[?25h" << std::flush;
}

void TUI::ansiMove(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

void TUI::ansiClearLine() {
    std::cout << "\033[K";
}

void TUI::ansiClearScreen() {
    std::cout << "\033[2J\033[H";
}

std::string TUI::centerText(const std::string& text, int w) {
    if ((int)text.size() >= w) return text.substr(0, w);
    int left = (w - (int)text.size()) / 2;
    return std::string(left, ' ') + text;
}

std::string TUI::currentTimeStr() {
    std::time_t now = std::time(nullptr);
    if (now != lastTimeUpdate) {
        lastTimeUpdate = now;
        char buf[16];
        std::strftime(buf, sizeof(buf), "%H:%M:%S", std::localtime(&now));
        cachedTimeStr = buf;
    }
    return cachedTimeStr;
}

void TUI::drawFrame() {
    // Row 0: top border
    ansiMove(1, 1);
    std::cout << TOP_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << TOP_RIGHT;

    // Content rows 1-4 (header, name area)
    for (int r = 1; r <= 4; r++) {
        ansiMove(r + 1, 1);
        std::cout << VERT;
        for (int i = 0; i < width; i++) std::cout << " ";
        std::cout << VERT;
    }

    // Row 5: separator
    ansiMove(6, 1);
    std::cout << LEFT_MID;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << RIGHT_MID;

    // Rows 6-7: progress, status
    for (int r = 6; r <= 7; r++) {
        ansiMove(r + 1, 1);
        std::cout << VERT;
        for (int i = 0; i < width; i++) std::cout << " ";
        std::cout << VERT;
    }

    // Row 8: bottom border
    ansiMove(9, 1);
    std::cout << BOT_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << BOT_RIGHT;

    frameBottomRow = 8;
}

void TUI::drawHeader(const std::string& modeDesc) {
    std::string title = i18n::Localizer::get(i18n::ID::TITLE_MAIN);
    std::string timeStr = currentTimeStr();

    std::string header = "  " + title;
    int pad = width - 2 - (int)title.size() - (int)modeDesc.size() - (int)timeStr.size() - 4;
    if (pad < 1) pad = 1;
    header += std::string(pad, ' ') + modeDesc + "  " + timeStr;
    if ((int)header.size() > width) header = header.substr(0, width);

    ansiMove(2, 2);
    ansiClearLine();
    std::cout << header;
}

void TUI::drawName(const std::string& name) {
    std::string centered = centerText(name, width);
    if ((int)centered.size() > width) centered = centered.substr(0, width);

    for (int r = 2; r <= 4; r++) {
        ansiMove(r + 1, 2);
        ansiClearLine();
    }

    ansiMove(4, 2);
    std::cout << centered;
}

void TUI::drawNextPreview(const std::string& nextName, bool hideNext) {
    std::string label = i18n::Localizer::get(i18n::ID::TUI_NEXT_LABEL);
    std::string display = hideNext ? i18n::Localizer::get(i18n::ID::TUI_HIDDEN) : nextName;
    std::string text = label + " " + display;
    if ((int)text.size() > width - 2) text = text.substr(0, width - 2);

    ansiMove(6, 2);
    ansiClearLine();
    std::cout << text;
}

void TUI::drawProgress(size_t current, size_t total) {
    int barWidth = width - 18;
    if (barWidth < 10) barWidth = 10;

    int filled = (total > 0) ? (int)((double)(current + 1) / total * barWidth) : 0;
    if (filled > barWidth) filled = barWidth;
    if (filled < 0) filled = 0;

    std::string bar;
    for (int i = 0; i < filled; i++) bar += PROGRESS_ON;
    for (int i = filled; i < barWidth; i++) bar += PROGRESS_OFF;

    std::ostringstream progressText;
    progressText << "  " << bar << "  " << (current + 1) << "/" << total;

    ansiMove(7, 2);
    ansiClearLine();
    std::cout << progressText.str();
}

void TUI::drawStatus(const std::string& msg) {
    std::string display = msg;
    if ((int)display.size() > width - 2) display = display.substr(0, width - 2);
    ansiMove(8, 2);
    ansiClearLine();
    if (!display.empty()) std::cout << display;
}

void TUI::showPickScreen(const std::string& name, size_t current, size_t total,
                          const std::string& modeDesc,
                          const std::string& nextName, bool hideNext) {
    if (!drawn) {
        ansiClearScreen();
        drawFrame();
        drawn = true;
    }

    drawHeader(modeDesc);
    drawName(name);
    drawNextPreview(nextName, hideNext);
    drawProgress(current, total);
    drawStatus(lastStatus);
    lastStatus.clear();

    std::cout << std::flush;
}

void TUI::showDoneScreen() {
    drawn = false;
    ansiClearScreen();

    std::string msg = i18n::Localizer::get(i18n::ID::TUI_DONE);
    std::string hint = i18n::Localizer::get(i18n::ID::TUI_HINT_DONE);

    std::cout << "\033[1;1H" << TOP_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << TOP_RIGHT;
    for (int r = 1; r <= 4; r++) {
        std::cout << "\033[" << (r + 1) << ";1H" << VERT;
        for (int i = 0; i < width; i++) std::cout << " ";
        std::cout << VERT;
    }
    std::cout << "\033[3;2H" << centerText(msg, width);
    std::cout << "\033[5;2H" << centerText(hint, width);
    std::cout << "\033[6;1H" << BOT_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << BOT_RIGHT;

    frameBottomRow = 5;
    std::cout << std::flush;
}

void TUI::showEmptyScreen() {
    drawn = false;
    ansiClearScreen();

    std::string msg = i18n::Localizer::get(i18n::ID::TUI_EMPTY);
    std::string hint = i18n::Localizer::get(i18n::ID::TUI_HINT_EXIT);

    std::cout << "\033[1;1H" << TOP_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << TOP_RIGHT;
    for (int r = 1; r <= 4; r++) {
        std::cout << "\033[" << (r + 1) << ";1H" << VERT;
        for (int i = 0; i < width; i++) std::cout << " ";
        std::cout << VERT;
    }
    std::cout << "\033[3;2H" << centerText(msg, width);
    std::cout << "\033[5;2H" << centerText(hint, width);
    std::cout << "\033[6;1H" << BOT_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << BOT_RIGHT;

    frameBottomRow = 5;
    std::cout << std::flush;
}

void TUI::drawPrompt() {
    int r = frameBottomRow + 2;
    ansiMove(r + 1, 1);
    ansiClearLine();
    std::cout << "> " << std::flush;
}

TUIAction TUI::parseCommand(const std::string& cmd) {
    std::string input = cmd;
    input.erase(0, input.find_first_not_of(" \t"));
    input.erase(input.find_last_not_of(" \t") + 1);
    std::transform(input.begin(), input.end(), input.begin(), ::tolower);

    if (input.empty()) return TUIAction::NONE;

    if (input == "next" || input == "n") return TUIAction::NEXT;
    if (input == "quit" || input == "q" || input == "exit") return TUIAction::QUIT;
    if (input == "hide" || input == "h") return TUIAction::HIDE;
    if (input == "all" || input == "mode all" || input == "mode_all") return TUIAction::MODE_ALL;
    if (input == "one" || input == "mode one" || input == "mode_one" || input == "mode one by one") return TUIAction::MODE_ONE;
    if (input == "lang" || input == "l") return TUIAction::LANG;
    if (input == "restart" || input == "r") return TUIAction::RESTART;
    if (input == "setup" || input == "config" || input == "settings" || input == "s") return TUIAction::SETUP;
    if (input == "help" || input == "?") {
        lastStatus = i18n::Localizer::get(i18n::ID::TUI_HELP);
        return TUIAction::NONE;
    }

    lastStatus = "Unknown: " + cmd + ". Type 'help' for commands.";
    return TUIAction::NONE;
}

TUIAction TUI::getAction() {
    drawPrompt();
    std::string line;
    std::getline(std::cin, line);
    return parseCommand(line);
}

}
