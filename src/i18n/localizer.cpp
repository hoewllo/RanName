#include "localizer.h"

namespace i18n {

Language Localizer::s_lang = Language::EN_US;

static const char* enUS[] = {
    "Random Name Picker",
    "Configuration",
    "Random Picking",
    "Name: ",
    "Remaining: ",
    "",
    "Invalid input, please try again.",
    "Press Enter to continue...",
    "The name list is empty. Please configure it first.",
    "Continue using? ",
    "Please wait, your ideal classroom random name picker is preparing...",
    "Current mode: ",
    "Name count: ",
    "You can configure via the setup tool.",
    "Please wait",
    "End",
    "Unable to open config file: ",
    "Unable to open config file for writing: ",
    "All Random",
    "One by One",
    "Unable to open name list file: ",
    "Current list (",
    " names):",
    "1. Add name",
    "2. Clear list",
    "3. Save and return",
    "Please choose: ",
    "Enter name: ",
    "List cleared.",
    "1. Edit list (",
    "2. Set mode (current: ",
    "3. Save and exit",
    "Choose mode:",
    "1. All Random",
    "2. One by One",
    "Settings saved.",
};

static const char* zhCN[] = {
    "随机点名程序",
    "随机点名配置",
    "随机点名",
    "姓名：",
    "剩余：",
    "人",
    "无效输入，请重新选择。",
    "按回车继续...",
    "名单为空，请先配置名单。",
    "继续使用吗？",
    "最后等待一下，您的理想课堂中的随机点名程序正在进行使用前的最后准备...",
    "当前点名模式：",
    "名单人数：",
    "您可以通过setup.exe进行设置。",
    "请稍后",
    "结束",
    "无法打开配置文件: ",
    "无法打开配置文件进行写入: ",
    "全局随机",
    "逐个随机",
    "无法打开名单文件: ",
    "当前名单 (",
    " 人):",
    "1. 添加姓名",
    "2. 清空名单",
    "3. 保存并返回",
    "请选择: ",
    "请输入姓名: ",
    "名单已清空。",
    "1. 编辑名单 (",
    "2. 设置模式 (当前: ",
    "3. 保存并退出",
    "选择模式:",
    "1. 全部覆盖的随机",
    "2. 逐个随机",
    "设置已保存。",
};

void Localizer::setLanguage(Language lang) {
    s_lang = lang;
}

Language Localizer::getLanguage() {
    return s_lang;
}

std::string Localizer::get(ID id) {
    int idx = static_cast<int>(id);
    if (idx < 0 || idx >= static_cast<int>(ID::_COUNT)) {
        return "";
    }
    switch (s_lang) {
        case Language::ZH_CN:
            return zhCN[idx];
        case Language::EN_US:
        default:
            return enUS[idx];
    }
}

Language Localizer::parseLanguage(const std::string& langStr) {
    if (langStr == "zh-CN" || langStr == "zh_CN" || langStr == "zh") {
        return Language::ZH_CN;
    }
    return Language::EN_US;
}

std::string Localizer::languageToString(Language lang) {
    switch (lang) {
        case Language::ZH_CN:
            return "zh_CN";
        case Language::EN_US:
        default:
            return "en_US";
    }
}

}
