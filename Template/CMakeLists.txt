
cmake_policy(SET CMP0017 NEW)

cmake_minimum_required(VERSION 3.1)

project (Template)

set(APP_NAME cocos2dcpp)

set(USE_CHIPMUNK 0)

set(COCOS_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../cocos2d)

set(EXTERNAL_DIR ${COCOS_ROOT}/external)

include(${COCOS_ROOT}/Config.cmake)

set(PRE_CC 0)

set(PRE_CC_DIR ${COCOS_ROOT}/prebuilt)

if(LINUX)
    set(PRE_CC_DIR ${PRE_CC_DIR}/Linux/${CMAKE_BUILD_TYPE}/${ARCH_DIR})
elseif(ANDROID)
    set(PRE_CC_DIR ${PRE_CC_DIR}/Android/${CMAKE_BUILD_TYPE}/${ANDROID_ABI})
endif()

if(USE_CHIPMUNK)
    set(PRE_CC_DIR ${PRE_CC_DIR}/Physics)
else()
    set(PRE_CC_DIR ${PRE_CC_DIR}/Normal)
endif()

if(LINUX)
    if(EXISTS "${PRE_CC_DIR}/libcocos2d.a" AND EXISTS "${PRE_CC_DIR}/libcocos2dInternal.a")
        set(PRE_CC 1)
    endif()
elseif(ANDROID)
    if(EXISTS "${PRE_CC_DIR}/libcocos2d.a"
            AND EXISTS "${PRE_CC_DIR}/libcocos2dInternal.a"
            AND EXISTS "${PRE_CC_DIR}/libcocos2dxandroid_static.a"
            AND EXISTS "${PRE_CC_DIR}/libcpufeatures.a")
        set(PRE_CC 1)
    endif()
endif()

if(NOT PRE_CC)
    if(LINUX)
        add_subdirectory(${COCOS_ROOT} ${COCOS_ROOT}/build/linux)
    elseif(ANDROID)
        add_subdirectory(${COCOS_ROOT} ${COCOS_ROOT}/build/android)
    endif()
endif()

set(PLATFORM_SPECIFIC_SRC)
if(LINUX)
    set(PLATFORM_SPECIFIC_SRC
        proj.linux/main.cpp
        )
elseif(ANDROID)
    set(PLATFORM_SPECIFIC_SRC
        app/src/main/cpp/main.cpp
        app/src/main/cpp/GamePayNative.cpp
        )
endif()

include_directories(
    ${COCOS_ROOT}/cocos
    ${COCOS_ROOT}/external
    ${COCOS_ROOT}/cocos/platform
    ${COCOS_ROOT}/cocos/audio/include/
    ${COCOS_ROOT}/cocos/editor-support
    Classes
    )

file(GLOB_RECURSE GAME_SRC "Classes/*.cpp")
file(GLOB_RECURSE GAME_HEAD "Classes/*.h")

if( ANDROID )
    add_library(${APP_NAME} SHARED ${GAME_SRC} ${GAME_HEAD} ${PLATFORM_SPECIFIC_SRC})
elseif(LINUX)
    add_executable(${APP_NAME} ${GAME_SRC} ${GAME_HEAD} ${PLATFORM_SPECIFIC_SRC})
endif()

if(NOT PRE_CC)
    target_link_libraries(${APP_NAME} cocos2d)
else()
    if(LINUX)
        target_link_libraries(${APP_NAME} ${PRE_CC_DIR}/libcocos2dInternal.a)
        target_link_libraries(${APP_NAME} ${PRE_CC_DIR}/libcocos2d.a)

        set(PLATFORM_LIBS jpeg png freetype fontconfig X11 fmod z pthread glfw GLEW GLU GL)
        target_link_libraries(${APP_NAME} ${PLATFORM_LIBS})
        if(USE_CHIPMUNK)
            target_link_libraries(${APP_NAME} ${EXTERNAL_DIR}/chipmunk/prebuilt/linux/${ARCH_DIR}/libchipmunk.a)
        endif()
    elseif(ANDROID)
        set(PLATFORM_LIBS GLESv1_CM GLESv2 EGL log android OpenSLES)
        target_link_libraries(${APP_NAME} ${PLATFORM_LIBS})

        target_link_libraries(${APP_NAME} ${PRE_CC_DIR}/libcocos2d.a)
        target_link_libraries(${APP_NAME} -Wl,-whole-archive ${PRE_CC_DIR}/libcocos2dxandroid_static.a -Wl,-no-whole-archive)
        target_link_libraries(${APP_NAME} ${PRE_CC_DIR}/libcocos2dInternal.a)
        target_link_libraries(${APP_NAME} ${PRE_CC_DIR}/libcpufeatures.a)

        target_link_libraries(${APP_NAME} ${EXTERNAL_DIR}/jpeg/prebuilt/android/${ANDROID_ABI}/libjpeg.a)
        target_link_libraries(${APP_NAME} ${EXTERNAL_DIR}/png/prebuilt/android/${ANDROID_ABI}/libpng.a)
        target_link_libraries(${APP_NAME} ${EXTERNAL_DIR}/freetype2/prebuilt/android/${ANDROID_ABI}/libfreetype.a)
        target_link_libraries(${APP_NAME} ${EXTERNAL_DIR}/zlib/prebuilt/android/${ANDROID_ABI}/libz.a)
        if(USE_CHIPMUNK)
            target_link_libraries(${APP_NAME} ${EXTERNAL_DIR}/chipmunk/prebuilt/android/${ANDROID_ABI}/libchipmunk.a)
        endif()
    endif()
endif()
