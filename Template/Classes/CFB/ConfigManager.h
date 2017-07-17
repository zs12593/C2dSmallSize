#pragma once

#include "cocos2d.h"

#define GCfm ConfigManager::getInstance()

USING_NS_CC;

class CFBReader {
public:
    static bool readBool(unsigned char *data, size_t &pos);

    static unsigned char readChar(unsigned char *data, size_t &pos);

    static int readInt(unsigned char *data, size_t &pos);

    static char *readUTF(unsigned char *data, size_t &pos);
};

class PurchaseInfo {
public:
    bool valid;
	bool hideValid;
	int money;
    const char *text;
	int hideAlpha;

    void read(unsigned char *data, size_t &pos);

    PurchaseInfo() {
        text = nullptr;
    }

    ~PurchaseInfo();
};

class ConfigBin {
public:
    int version;
	const char *projName;
    const char *sdkName;
    const char *sdkText;
	const char *apkName;
	const char *pkg;
	size_t sign;
	int hideAlpha;
    size_t argsCount;
    char **args;
    size_t infoCount;
	PurchaseInfo **infos;

    void read(unsigned char *data, size_t &pos);

    ~ConfigBin();
};

class ConfigManager {
public:
    static ConfigManager *getInstance();

    const char *getProjName();

    const char *getApkName();

    const char *getSdkName();

    const char *getText(int payId);

    const char *getArg(int id);

	size_t getArgsCount();

    const char *getPkgName();

    size_t getSign();

    bool isValid(int payId);

	int getMoney(int payId);

	int getHideAlpha(int payId);

    bool isHideValid(int payId);

private:
    ConfigBin *cfb;

    ConfigManager();

    ~ConfigManager();
};
