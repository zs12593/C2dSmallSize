/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "editor-support/cocostudio/CSLoader.h"
#include "base/ObjectFactory.h"
#include "base/CCDirector.h"
#include "base/ccUTF8.h"
#include "ui/CocosGUI.h"
#include "2d/CCSpriteFrameCache.h"
#include "2d/CCParticleSystemQuad.h"
#include "2d/CCTMXTiledMap.h"
#include "platform/CCFileUtils.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "editor-support/cocostudio/CSParseBinary_generated.h"

#include "editor-support/cocostudio/WidgetReader/NodeReaderProtocol.h"
#include "editor-support/cocostudio/WidgetReader/NodeReaderDefine.h"

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
#include "editor-support/cocostudio/WidgetReader/TabControlReader/TabControlReader.h"

#include "flatbuffers/flatbuffers.h"
#include "flatbuffers/util.h"

#include "editor-support/cocostudio/FlatBuffersSerialize.h"

#include <fstream>

using namespace cocos2d::ui;
using namespace cocostudio;
using namespace flatbuffers;

NS_CC_BEGIN

Node* CSLoader::createNode(const std::string &filename) {
	static bool inited = false;
	if (!inited) {
		inited = true;
		CREATE_CLASS_NODE_READER_INFO(NodeReader);
		CREATE_CLASS_NODE_READER_INFO(SingleNodeReader);
		CREATE_CLASS_NODE_READER_INFO(SpriteReader);
		CREATE_CLASS_NODE_READER_INFO(GameMapReader);

		CREATE_CLASS_NODE_READER_INFO(ButtonReader);
		CREATE_CLASS_NODE_READER_INFO(CheckBoxReader);
		CREATE_CLASS_NODE_READER_INFO(ImageViewReader);
		CREATE_CLASS_NODE_READER_INFO(TextBMFontReader);
		CREATE_CLASS_NODE_READER_INFO(TextReader);
		CREATE_CLASS_NODE_READER_INFO(TextFieldReader);
		CREATE_CLASS_NODE_READER_INFO(TextAtlasReader);
		CREATE_CLASS_NODE_READER_INFO(LoadingBarReader);
		CREATE_CLASS_NODE_READER_INFO(SliderReader);
		CREATE_CLASS_NODE_READER_INFO(LayoutReader);
		CREATE_CLASS_NODE_READER_INFO(ScrollViewReader);
		CREATE_CLASS_NODE_READER_INFO(PageViewReader);
		CREATE_CLASS_NODE_READER_INFO(ListViewReader);
		CREATE_CLASS_NODE_READER_INFO(TabControlReader);
	}
	std::string path = filename;
	size_t pos = path.find_last_of('.');
	std::string suffix = path.substr(pos + 1, path.length());

	if (suffix == "csb") {
		CSLoader load;
		return load.createNodeWithFlatBuffersFile(filename);
	}

	return nullptr;
}

Node* CSLoader::createNodeWithFlatBuffersFile(const std::string &filename)
{
    Node* node = nodeWithFlatBuffersFile(filename);
    return node;
}

Node* CSLoader::nodeWithFlatBuffersFile(const std::string &fileName)
{
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(fileName);
    
    CC_ASSERT(FileUtils::getInstance()->isFileExist(fullPath));
    
    Data buf = FileUtils::getInstance()->getDataFromFile(fullPath);

    if (buf.isNull())
    {
        CCLOG("CSLoader::nodeWithFlatBuffersFile - failed read file: %s", fileName.c_str());
        CC_ASSERT(false);
        return nullptr;
    }

    auto csparsebinary = GetCSParseBinary(buf.getBytes());
    // decode plist
    auto textures = csparsebinary->textures();
    int textureSize = textures->size();
    for (int i = 0; i < textureSize; ++i)
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(textures->Get(i)->c_str());
    
    Node* node = nodeWithFlatBuffers(csparsebinary->nodeTree());
    
    return node;
}

Node* CSLoader::nodeWithFlatBuffers(const flatbuffers::NodeTree *nodetree) {
	if (nodetree == nullptr)
		return nullptr;
	Node* node = nullptr;

	std::string classname = nodetree->classname()->c_str();

	auto options = nodetree->options();

	std::string customClassName = nodetree->customClassName()->c_str();
	if (customClassName != "") 
		classname = customClassName;
	std::string readername = getGUIClassName(classname);
	readername.append("Reader");
	NodeReaderProtocol* reader = dynamic_cast<NodeReaderProtocol*>(ObjectFactory::getInstance()->createObject(readername));
	if (reader)
		node = reader->createNodeWithFlatBuffers(options->data());
	
	if (!node) {
		return nullptr;
	}

	auto children = nodetree->children();
	int size = children->size();
	for (int i = 0; i < size; ++i) {
		auto subNodeTree = children->Get(i);
		Node* child = nodeWithFlatBuffers(subNodeTree);
		if (child) {
			PageView* pageView = dynamic_cast<PageView*>(node);
			ListView* listView = dynamic_cast<ListView*>(node);
			if (pageView) {
				Layout* layout = dynamic_cast<Layout*>(child);
				if (layout) {
					pageView->addPage(layout);
				}
			} else if (listView) {
				Widget* widget = dynamic_cast<Widget*>(child);
				if (widget) {
					listView->pushBackCustomItem(widget);
				}
			} else {
				node->addChild(child);
			}
		}
	}

	return node;
}

std::string CSLoader::getGUIClassName(const std::string &name)
{
    std::string convertedClassName = name;
    if (name == "Panel")
    {
        convertedClassName = "Layout";
    }
    else if (name == "TextArea")
    {
        convertedClassName = "Text";
    }
    else if (name == "TextButton")
    {
        convertedClassName = "Button";
    }
    else if (name == "Label")
    {
        convertedClassName = "Text";
    }
    else if (name == "LabelAtlas")
    {
        convertedClassName = "TextAtlas";
    }
    else if (name == "LabelBMFont")
    {
        convertedClassName = "TextBMFont";
    }
    
    return convertedClassName;
}

NS_CC_END
