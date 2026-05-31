#ifndef LOCALIZER_H
#define LOCALIZER_H

#include <string>

namespace i18n {

enum class ID {
    TITLE_MAIN,
    TITLE_CONFIG,
    TITLE_PICKING,
    NAME_LABEL,
    REMAINING_LABEL,
    PERSON_UNIT,
    INVALID_INPUT,
    PRESS_ENTER,
    LIST_EMPTY,
    CONTINUE_ASK,
    PREPARING,
    CURRENT_MODE,
    NAME_COUNT,
    SETUP_HINT,
    PLEASE_WAIT,
    END,
    UNABLE_OPEN_CONFIG,
    UNABLE_WRITE_CONFIG,
    MODE_ALL_RANDOM,
    MODE_ONE_BY_ONE,
    UNABLE_OPEN_LIST,
    CURRENT_LIST,
    NAMES_COUNT,
    ADD_NAME,
    CLEAR_LIST,
    SAVE_RETURN,
    PLEASE_CHOOSE,
    ENTER_NAME,
    LIST_CLEARED,
    EDIT_LIST,
    SET_MODE,
    SAVE_EXIT,
    CHOOSE_MODE,
    MODE_1_ALL,
    MODE_2_ONE,
    SETTINGS_SAVED,
    TUI_HELP,
    TUI_DONE,
    TUI_EMPTY,
    TUI_HIDDEN,
    _COUNT
};

enum class Language {
    EN_US,
    ZH_CN
};

class Localizer {
public:
    static void setLanguage(Language lang);
    static Language getLanguage();
    static std::string get(ID id);

    static Language parseLanguage(const std::string& langStr);
    static std::string languageToString(Language lang);

private:
    static Language s_lang;
};

}

#endif
