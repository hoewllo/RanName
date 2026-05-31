#include "platform.h"
#include "../i18n/localizer.h"
#include <iostream>
#include <cstdio>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
    #include <conio.h>
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/ioctl.h>
    #include <termios.h>
#endif

namespace utils {

#ifdef _WIN32
    static HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    static DWORD origConsoleMode = 0;
#else
    static struct termios origTermios;
    static bool termiosSaved = false;
#endif

    void Platform::setConsoleTitle(const std::string& title) {
        #ifdef _WIN32
            system(("title " + title).c_str());
        #endif
    }

    void Platform::clearScreen() {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void Platform::setUTF8Encoding() {
        #ifdef _WIN32
            system("chcp 65001 > nul");
        #endif
        std::string locale = "en_US.UTF-8";
        if (i18n::Localizer::getLanguage() == i18n::Language::ZH_CN) {
            locale = "zh_CN.UTF-8";
        }
        std::setlocale(LC_ALL, locale.c_str());
    }

    void Platform::sleep(unsigned int milliseconds) {
        #ifdef _WIN32
            Sleep(milliseconds);
        #else
            usleep(milliseconds * 1000);
        #endif
    }

    void Platform::createDirectory(const std::string& path) {
        #ifdef _WIN32
            system(("if not exist " + path + " md " + path).c_str());
        #else
            system(("mkdir -p " + path).c_str());
        #endif
    }

    int Platform::getTerminalWidth() {
        #ifdef _WIN32
            CONSOLE_SCREEN_BUFFER_INFO csbi;
            if (GetConsoleScreenBufferInfo(hStdout, &csbi)) {
                return csbi.srWindow.Right - csbi.srWindow.Left + 1;
            }
            return 80;
        #else
            struct winsize ws;
            if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == 0 && ws.ws_col > 0) {
                return ws.ws_col;
            }
            return 80;
        #endif
    }

    void Platform::setRawMode(bool enable) {
        #ifdef _WIN32
            if (enable) {
                DWORD mode = 0;
                GetConsoleMode(hStdout, &origConsoleMode);
                GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
                mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
                SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
            } else if (origConsoleMode) {
                SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), origConsoleMode);
            }
        #else
            if (enable) {
                struct termios raw;
                tcgetattr(STDIN_FILENO, &origTermios);
                termiosSaved = true;
                raw = origTermios;
                cfmakeraw(&raw);
                tcsetattr(STDIN_FILENO, TCSANOW, &raw);
            } else if (termiosSaved) {
                tcsetattr(STDIN_FILENO, TCSANOW, &origTermios);
                termiosSaved = false;
            }
        #endif
    }

    char Platform::getChar() {
        #ifdef _WIN32
            return _getch();
        #else
            return getchar();
        #endif
    }
}
