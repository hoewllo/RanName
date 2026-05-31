#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>

namespace utils {
    class Platform {
    public:
        static void setConsoleTitle(const std::string& title);
        static void clearScreen();
        static void setUTF8Encoding();
        static void sleep(unsigned int milliseconds);
        static void createDirectory(const std::string& path);
    };
}

#endif // PLATFORM_H