#include "GamePay.h"
#include "CFB/ConfigManager.h"

static bool isHide = false;

std::string& replaceAll(std::string& str, const std::string& old_value, const std::string& new_value)
{
    for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
        if ((pos = str.find(old_value, pos)) != std::string::npos)
            str.replace(pos, old_value.length(), new_value);
        else
            break;
    }
    return str;
}

bool GamePay::isSdkHide() {
    return isHide;
}

void GamePay::setSdkHide(bool hide) {
    isHide = hide;
}

void GamePay::buy(int payId) {
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_LINUX
    onBuyCallback(payId, true);
#else
    char temp[20];
    sprintf(temp, "product%d", payId);
    JniHelper::callStaticVoidMethod("abc.abc.abc.GamePay", "pay", payId,
                                    ConfigManager::getInstance()->getMoney(payId), "Product");
#endif
}

void GamePay::onBuyCallback(int payId, bool success) {
	int userData[2] = {payId, success ? 1 : 0};
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent("EVENT_BUY", (void*)&userData);
}

std::string GamePay::getString(int payId) {
    std::string text = std::string(ConfigManager::getInstance()->getText(payId));
    char temp[20];
    sprintf(temp, "product%d", payId);
    text = replaceAll(text, "{product}", "Product");
    text = replaceAll(text, "{info}", "Info");
    text = replaceAll(text, "\\n", "\n");
    sprintf(temp, "%d", ConfigManager::getInstance()->getMoney(payId));
    text = replaceAll(text, "{price}", temp);
    return text;
}

const char *GamePay::getArg(int index) {
	return ConfigManager::getInstance()->getArg(index);
}
