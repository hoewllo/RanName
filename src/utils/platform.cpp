#include "platform.h"
#include <iostream>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
#else
    #include <unistd.h>
    #include <sys/stat.h>
#endif

namespace utils {
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
        std::setlocale(LC_ALL, "zh_CN.UTF-8");
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
}