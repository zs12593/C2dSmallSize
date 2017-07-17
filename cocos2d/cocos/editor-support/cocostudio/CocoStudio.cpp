
#include "editor-support/cocostudio/CocoStudio.h"

#include "editor-support/cocostudio/WidgetReader/NodeReader/NodeReader.h"
#include "editor-support/cocostudio/WidgetReader/SingleNodeReader/SingleNodeReader.h"
#include "editor-support/cocostudio/WidgetReader/SpriteReader/SpriteReader.h"
#include "editor-support/cocostudio/WidgetReader/GameMapReader/GameMapReader.h"

#include "editor-support/cocostudio/WidgetReader/ButtonReader/ButtonReader.h"
#include "editor-support/cocostudio/WidgetReader/CheckBoxReader/CheckBoxReader.h"
#include "editor-support/cocostudio/WidgetReader/ImageViewReader/ImageViewReader.h"
#include "editor-support/cocostudio/WidgetReader/TextBMFontReader/TextBMFontReader.h"
#include "editor-support/cocostudio/WidgetReader/TextReader/TextReader.h"
#include "editor-support/cocostudio/WidgetReader/TextFieldReader/TextFieldReader.h"
#include "editor-support/cocostudio/WidgetReader/TextAtlasReader/TextAtlasReader.h"
#include "editor-support/cocostudio/WidgetReader/LoadingBarReader/LoadingBarReader.h"
#include "editor-support/cocostudio/WidgetReader/SliderReader/SliderReader.h"
#include "editor-support/cocostudio/WidgetReader/LayoutReader/LayoutReader.h"
#include "editor-support/cocostudio/WidgetReader/ScrollViewReader/ScrollViewReader.h"
#include "editor-support/cocostudio/WidgetReader/PageViewReader/PageViewReader.h"
#include "editor-support/cocostudio/WidgetReader/ListViewReader/ListViewReader.h"


namespace cocostudio
{
    void destroyCocosStudio()
    {        
        NodeReader::destroyInstance();
        SingleNodeReader::destroyInstance();
        SpriteReader::destroyInstance();
        GameMapReader::destroyInstance();
        
        WidgetReader::destroyInstance();
        ButtonReader::destroyInstance();
        CheckBoxReader::destroyInstance();
        ImageViewReader::destroyInstance();
        TextBMFontReader::destroyInstance();
        TextReader::destroyInstance();
        TextFieldReader::destroyInstance();
        TextAtlasReader::destroyInstance();
        LoadingBarReader::destroyInstance();
        SliderReader::destroyInstance();
        LayoutReader::destroyInstance();
        ScrollViewReader::destroyInstance();
        PageViewReader::destroyInstance();
        ListViewReader::destroyInstance();
    }
}