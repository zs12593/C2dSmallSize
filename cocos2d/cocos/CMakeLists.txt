
cmake_minimum_required(VERSION 3.1)

project(cocos2dInternal)

include(2d/CMakeLists.txt)
include(platform/CMakeLists.txt)
include(physics/CMakeLists.txt)
include(math/CMakeLists.txt)
include(renderer/CMakeLists.txt)
include(base/CMakeLists.txt)
include(deprecated/CMakeLists.txt)
include(ui/CMakeLists.txt)
include(audio/CMakeLists.txt)
include(storage/CMakeLists.txt)
if(USE_CCS)
  include(editor-support/cocostudio/CMakeLists.txt)
endif()
include(../extensions/CMakeLists.txt)
include(../external/tinyxml2/CMakeLists.txt)
include(../external/flatbuffers/CMakeLists.txt)
include(../external/xxhash/CMakeLists.txt)
include(../external/unzip/CMakeLists.txt)

set(COCOS_SRC cocos2d.cpp
    ${COCOS_2D_SRC}
    ${COCOS_PLATFORM_SRC}
    ${COCOS_PHYSICS_SRC}
    ${COCOS_MATH_SRC}
    ${COCOS_RENDERER_SRC}
    ${COCOS_BASE_SRC}
    ${COCOS_AUDIO_SRC}
    ${COCOS_UI_SRC}
    ${COCOS_DEPRECATED_SRC}
    ${COCOS_EXTENSIONS_SRC}
    ${TINYXML2_SRC}
    ${FLATBUFFERS_SRC}
    ${XXHASH}
    ${UNZIP_SRC}
    )

if(USE_CCS)
  set(COCOS_SRC ${COCOS_SRC} ${COCOS_CS_SRC})
endif()

include_directories(../external/xxtea)
include_directories(../external/clipper)
include_directories(../external/xxhash)
include_directories(../external/tinyxml2)
include_directories(../external/flatbuffers)
include_directories(../external/unzip)

add_library(cocos2dInternal ${COCOS_SRC})

set(PLATFORM_LIBS)
if(USE_CHIPMUNK)
    set(PLATFORM_LIBS chipmunk)
endif()
if(LINUX)
    set(PLATFORM_LIBS ${PLATFORM_LIBS} jpeg png fontconfig freetype X11 fmod z pthread glfw GLEW GLU GL)
elseif(ANDROID)
  set(PLATFORM_LIBS ${PLATFORM_LIBS} GLESv1_CM GLESv2 EGL log android OpenSLES)

  target_link_libraries(cocos2dInternal ${EXTERNAL_DIR}/jpeg/prebuilt/android/${ANDROID_ABI}/libjpeg.a)
  target_link_libraries(cocos2dInternal ${EXTERNAL_DIR}/png/prebuilt/android/${ANDROID_ABI}/libpng.a)
  target_link_libraries(cocos2dInternal ${EXTERNAL_DIR}/freetype2/prebuilt/android/${ANDROID_ABI}/libfreetype.a)
  target_link_libraries(cocos2dInternal ${EXTERNAL_DIR}/zlib/prebuilt/android/${ANDROID_ABI}/libz.a)

  include_directories(${ANDROID_NDK}/sources/android/cpufeatures)
  add_library(cpufeatures ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)
  target_link_libraries(cpufeatures dl)
endif()

target_link_libraries(cocos2dInternal ${PLATFORM_LIBS})

add_library(cocos2d cc_dummy.c)

if(ANDROID)
  target_link_libraries(cocos2d -Wl,-whole-archive cocos2dxandroid_static -Wl,-no-whole-archive)
  target_link_libraries(cocos2d cocos2dInternal cpufeatures)
else()
  target_link_libraries(cocos2d cocos2dInternal)
endif()
