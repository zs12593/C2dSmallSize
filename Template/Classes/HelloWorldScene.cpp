#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::create();
    auto layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

bool HelloWorld::init()
{
    if (!Layer::init())
        return false;
	
	Label* label = Label::create("Hello World", "Arail", 24);
	label->setPosition(400, 240);
	this->addChild(label);
    
    return true;
}

