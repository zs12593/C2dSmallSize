/**
 Copyright 2013 BlackBerry Inc.
 Copyright (c) 2015 Chukong Technologies

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 Original file from GamePlay3D: http://gameplay3d.org

 This file was modified to fit the cocos2d-x project
 */

#include "renderer/CCVertexAttribBinding.h"
#include "renderer/CCGLProgramState.h"
#include "renderer/ccGLStateCache.h"
#include "platform/CCGL.h"
#include "base/CCConfiguration.h"

NS_CC_BEGIN

std::string s_attributeNames[] = {
    GLProgram::ATTRIBUTE_NAME_POSITION,
    GLProgram::ATTRIBUTE_NAME_COLOR,
    GLProgram::ATTRIBUTE_NAME_TEX_COORD,
    GLProgram::ATTRIBUTE_NAME_TEX_COORD1,
    GLProgram::ATTRIBUTE_NAME_TEX_COORD2,
    GLProgram::ATTRIBUTE_NAME_TEX_COORD3,
    GLProgram::ATTRIBUTE_NAME_NORMAL,
    GLProgram::ATTRIBUTE_NAME_BLEND_WEIGHT,
    GLProgram::ATTRIBUTE_NAME_BLEND_INDEX,
    GLProgram::ATTRIBUTE_NAME_TANGENT,
    GLProgram::ATTRIBUTE_NAME_BINORMAL
};

static GLuint __maxVertexAttribs = 0;
static std::vector<VertexAttribBinding*> __vertexAttribBindingCache;

VertexAttribBinding::VertexAttribBinding() :
    _handle(0), _attributes(), _glProgramState(nullptr)
{
}

VertexAttribBinding::~VertexAttribBinding()
{
    // Delete from the vertex attribute binding cache.
    std::vector<VertexAttribBinding*>::iterator itr = std::find(__vertexAttribBindingCache.begin(), __vertexAttribBindingCache.end(), this);
    if (itr != __vertexAttribBindingCache.end())
    {
        __vertexAttribBindingCache.erase(itr);
    }

    CC_SAFE_RELEASE(_glProgramState);
    _attributes.clear();

    if (_handle)
    {
        glDeleteVertexArrays(1, &_handle);
        _handle = 0;
    }
}

VertexAttribBinding* VertexAttribBinding::create(GLProgramState* glProgramState)
{
    CCASSERT(glProgramState, "Invalid GLProgramState");

    // Search for an existing vertex attribute binding that can be used.
    VertexAttribBinding* b;
    for (size_t i = 0, count = __vertexAttribBindingCache.size(); i < count; ++i)
    {
        b = __vertexAttribBindingCache[i];
        CC_ASSERT(b);
        if (b->_glProgramState == glProgramState)
        {
            // Found a match!
            return b;
        }
    }

    b = new (std::nothrow) VertexAttribBinding();
    if (b && b->init(glProgramState))
    {
        b->autorelease();
        __vertexAttribBindingCache.push_back(b);
    }

    return b;
}

bool VertexAttribBinding::init(GLProgramState* glProgramState)
{
    CCASSERT(glProgramState, "Invalid arguments");

    // One-time initialization.
    if (__maxVertexAttribs == 0)
    {
        GLint temp;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &temp);

        __maxVertexAttribs = temp;
        if (__maxVertexAttribs <= 0)
        {
            CCLOGERROR("The maximum number of vertex attributes supported by OpenGL on the current device is 0 or less.");
            return false;
        }
    }

    _glProgramState = glProgramState;
    _glProgramState->retain();

    // Parse and set attributes
    parseAttributes();

    return true;
}

void VertexAttribBinding::bind()
{

    if (_handle)
    {
        // hardware
        GL::bindVAO(_handle);
    }
    else
    {
        // Software mode
        GL::enableVertexAttribs(_vertexAttribsFlags);
        // set attributes
        for(auto &attribute : _attributes)
        {
            attribute.second.apply();
        }
        
    }
}

void VertexAttribBinding::unbind()
{
    if (_handle)
    {
        // Hardware
       GL::bindVAO(0);
    }
    else
    {
        // Software
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}

uint32_t VertexAttribBinding::getVertexAttribsFlags() const
{
    return _vertexAttribsFlags;
}

void VertexAttribBinding::parseAttributes()
{
    CCASSERT(_glProgramState, "invalid glprogram");

    _attributes.clear();
    _vertexAttribsFlags = 0;

    auto glprogram = _glProgramState->getGLProgram();

    for(auto &attrib: glprogram->_vertexAttribs)
    {
        VertexAttribValue value(&attrib.second);
        _attributes[attrib.first] = value;
    }
}

VertexAttribValue* VertexAttribBinding::getVertexAttribValue(const std::string& name)
{
    const auto itr = _attributes.find(name);
    if( itr != _attributes.end())
        return &itr->second;
    return nullptr;
}

void VertexAttribBinding::setVertexAttribPointer(const std::string &name, GLint size, GLenum type, GLboolean normalized, GLsizei stride, GLvoid* pointer)
{
    auto v = getVertexAttribValue(name);
    if(v) {
        v->setPointer(size, type, normalized, stride, pointer);
        _vertexAttribsFlags |= 1 << v->_vertexAttrib->index;
    }
    else
    {
        CCLOG("cocos2d: warning: Attribute not found: %s", name.c_str());
    }
}

NS_CC_END
