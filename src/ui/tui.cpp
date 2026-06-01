#include "tui.h"
#include "../utils/platform.h"
#include "../i18n/localizer.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include <cstdio>

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

static void ansiMove(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

static void ansiClearLine() {
    std::cout << "\033[K";
}

static void ansiClearScreen() {
    std::cout << "\033[2J\033[H";
}

TUI::TUI()
    : width(78)
    , height(24)
    , lastTimeUpdate(0)
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

void TUI::moveTo(int row, int col) {
    ansiMove(row + 1, col + 1);
}

void TUI::clearLine(int row) {
    ansiMove(row + 1, 1);
    ansiClearLine();
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
    std::cout << TOP_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << TOP_RIGHT << std::endl;

    // Empty content rows with side borders
    for (int i = 0; i < 15; i++) {
        std::cout << VERT;
        for (int j = 0; j < width; j++) std::cout << " ";
        std::cout << VERT << std::endl;
    }

    std::cout << LEFT_MID;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << RIGHT_MID << std::endl;

    // Progress rows
    for (int i = 0; i < 2; i++) {
        std::cout << VERT;
        for (int j = 0; j < width; j++) std::cout << " ";
        std::cout << VERT << std::endl;
    }

    std::cout << BOT_LEFT;
    for (int i = 0; i < width; i++) std::cout << HORIZ;
    std::cout << BOT_RIGHT << std::endl;
}

void TUI::drawHeader(const std::string& modeDesc) {
    // Line 0: top border (already drawn)
    // Line 1: header content
    std::string title = i18n::Localizer::get(i18n::ID::TITLE_MAIN);
    std::string timeStr = currentTimeStr();
    std::string header = "  " + title;
    header += std::string(width - 4 - (int)title.size() - (int)modeDesc.size() - (int)timeStr.size() - 2, ' ');
    header += modeDesc + "  " + timeStr;
    if ((int)header.size() > width) header = header.substr(0, width);

    ansiMove(2, 2);
    std::cout << header;
}

void TUI::drawNameArea(const std::string& name, bool hideNext) {
    std::string displayName = hideNext ? i18n::Localizer::get(i18n::ID::TUI_HIDDEN) : name;

    int midRow = 9;
    std::string centered = centerText(displayName, width);
    if ((int)centered.size() > width) centered = centered.substr(0, width);

    ansiMove(midRow, 2);
    std::cout << centered;
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

    // Row 18: progress bar (first row after separator)
    ansiMove(18, 2);
    std::cout << progressText.str();
    ansiClearLine();
}

void TUI::drawControls() {
    std::string help = i18n::Localizer::get(i18n::ID::TUI_HELP);
    if ((int)help.size() > width) help = help.substr(0, width);
    ansiMove(19, 2);
    std::cout << help;
    ansiClearLine();
}

void TUI::showPickScreen(const std::string& name, size_t current, size_t total,
                          const std::string& modeDesc, bool hideNext) {
    if (current == 0) {
        ansiClearScreen();
        drawFrame();
        utils::Platform::setRawMode(true);
    }

    drawHeader(modeDesc);
    drawNameArea(name, hideNext);
    drawProgress(current, total);
    drawControls();
    drawTime();
    std::cout << std::flush;
}

void TUI::showDoneScreen() {
    ansiMove(9, 2);
    ansiClearLine();
    std::string msg = i18n::Localizer::get(i18n::ID::TUI_DONE);
    std::string centered = centerText(msg, width);
    if ((int)centered.size() > width) centered = centered.substr(0, width);
    std::cout << centered << std::flush;

    utils::Platform::setRawMode(false);
    utils::Platform::getChar();
    std::cout << std::endl;
}

void TUI::showEmptyScreen() {
    ansiClearScreen();
    drawFrame();
    ansiMove(9, 2);
    std::string msg = i18n::Localizer::get(i18n::ID::TUI_EMPTY);
    std::string centered = centerText(msg, width);
    if ((int)centered.size() > width) centered = centered.substr(0, width);
    std::cout << centered << std::flush;

    utils::Platform::setRawMode(true);
    utils::Platform::getChar();
    utils::Platform::setRawMode(false);
    std::cout << std::endl;
}

void TUI::drawTime() {
    // Time is shown in the header, updated on each draw
}

TUIAction TUI::getAction() {
    char c = utils::Platform::getChar();
    switch (c) {
        case ' ':
        case '\n':
        case '\r':
            return TUIAction::NEXT;
        case 'q':
        case 'Q':
            return TUIAction::QUIT;
        case 'h':
        case 'H':
            return TUIAction::HIDE;
        case 'a':
        case 'A':
            return TUIAction::MODE_ALL;
        case 'o':
        case 'O':
            return TUIAction::MODE_ONE;
        case 'l':
        case 'L':
            return TUIAction::LANG;
        default:
            return TUIAction::NONE;
    }
}

}
