#include "platform.h"
#include "../i18n/localizer.h"
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
}
