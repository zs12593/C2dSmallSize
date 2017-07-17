#pragma once

#include "Res.h"

#include "cocos2d.h"
#define GGetString(__ARG__) Strings::getInstance()->getString(__ARG__)

class Strings {
public:
    static Strings *getInstance() {
        static Strings *sd = new Strings;
        return sd;
    }

    const char *getString(const std::string &key) {
        return vm[key].asString().c_str();
    }

private:
    cocos2d::ValueMap vm;

    Strings() {
        vm = cocos2d::FileUtils::getInstance()->getValueMapFromFile("Data/Strings.plist");
    }
};
