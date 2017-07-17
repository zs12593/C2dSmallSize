#include "ConfigManager.h"

bool CFBReader::readBool(unsigned char *data, size_t &pos) {
    return data[pos++] != 0;
}

unsigned char CFBReader::readChar(unsigned char *data, size_t &pos) {
    return data[pos++];
}

int CFBReader::readInt(unsigned char *data, size_t &pos) {
    unsigned char i1 = data[pos++];
    unsigned char i2 = data[pos++];
    unsigned char i3 = data[pos++];
    unsigned char i4 = data[pos++];
    int ret = (i1 << 24) + (i2 << 16) + (i3 << 8) + i4;
    return ret;
}

char *CFBReader::readUTF(unsigned char *data, size_t &pos) {
    unsigned char i1 = data[pos++];
    unsigned char i2 = data[pos++];
    int len = (i1 << 8) + i2;
    if (len == 0)
        return nullptr;
    char *ret = new char[len + 1];
    int index = 0;
    while (index < len)
        ret[index++] = data[pos++];
    ret[index] = 0;
    return ret;
}

void PurchaseInfo::read(unsigned char *data, size_t &pos) {
    valid = CFBReader::readBool(data, pos);
    hideValid = CFBReader::readBool(data, pos);
    money = CFBReader::readInt(data, pos);
    text = CFBReader::readUTF(data, pos);
    hideAlpha = CFBReader::readInt(data, pos);
}

PurchaseInfo::~PurchaseInfo() {
    if (text)
        delete[] text;
}

void ConfigBin::read(unsigned char *data, size_t &pos) {
    version = CFBReader::readInt(data, pos);
    projName = CFBReader::readUTF(data, pos);
    sdkName = CFBReader::readUTF(data, pos);
    sdkText = CFBReader::readUTF(data, pos);
    apkName = CFBReader::readUTF(data, pos);
    pkg = CFBReader::readUTF(data, pos);
    sign = CFBReader::readChar(data, pos);
    hideAlpha = CFBReader::readInt(data, pos);
    argsCount = CFBReader::readChar(data, pos);
    if (argsCount > 0) {
        args = new char *[argsCount];
        for (size_t i = 0; i < argsCount; i++)
            args[i] = CFBReader::readUTF(data, pos);
    }
    infoCount = CFBReader::readChar(data, pos);
    if (infoCount > 0) {
        infos = new PurchaseInfo *[infoCount];
        for (size_t i = 0; i < infoCount; i++) {
            infos[i] = new PurchaseInfo;
            infos[i]->read(data, pos);
        }
    }
}

ConfigBin::~ConfigBin() {
    delete[] projName;
    delete[] apkName;
    delete[] sdkName;
    delete[] sdkText;
    delete[] pkg;
    if (argsCount > 0)
        delete[] args;
    if (infoCount > 0) {
        delete[] infos;
    }
}

ConfigManager *ConfigManager::getInstance() {
    static ConfigManager *cfgm = new ConfigManager;
    return cfgm;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

const char *ConfigManager::getProjName() {
    return cfb->projName;
}

const char *ConfigManager::getApkName() {
    return cfb->apkName;
}

const char *ConfigManager::getSdkName() {
    return cfb->sdkName;
}

const char *ConfigManager::getText(int payId) {
    if (cfb->infos[payId]->text)
        return cfb->infos[payId]->text;
    return cfb->sdkText;
}

const char *ConfigManager::getArg(int id) {
    return cfb->args[id];
}

size_t ConfigManager::getArgsCount()
{
    return cfb->argsCount;
}

const char *ConfigManager::getPkgName() {
    return cfb->pkg;
}

size_t ConfigManager::getSign() {
    return cfb->sign;
}

bool ConfigManager::isValid(int payId) {
    return cfb->infos[payId]->valid;
}

int ConfigManager::getMoney(int payId) {
    return cfb->infos[payId]->money;
}

int ConfigManager::getHideAlpha(int payId)
{
    if(cfb->infos[payId]->hideAlpha <= 255)
        return cfb->infos[payId]->hideAlpha;
    return cfb->hideAlpha;
}

bool ConfigManager::isHideValid(int payId) {
    return cfb->infos[payId]->hideValid;
}

ConfigManager::ConfigManager() {
    Data d = FileUtils::getInstance()->getDataFromFile("Config.bin");
    cfb = new ConfigBin;
    size_t pos = 0;
    cfb->read(d.getBytes(), pos);
}

ConfigManager::~ConfigManager() {
    delete cfb;
}
