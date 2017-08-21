
set(USE_CCS 1)

if(${CMAKE_BUILD_TYPE} MATCHES "Release")
    ADD_DEFINITIONS (-DRELEASE)
endif()

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  if(ANDROID)
    ADD_DEFINITIONS (-DUSE_FILE32API)
  else()
    set(LINUX TRUE)
    # add_definitions(-DLINUX -DN__APPLE__)

    set(CMAKE_C_FLAGS_DEBUG "-g -Wall -Wextra -DCOCOS2D_DEBUG=1 -DLINUX -DN__APPLE__ -Wno-int-to-void-pointer-cast")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -std=c++11")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99 -fPIC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fPIC")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
      set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()

  endif()
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Android")
    ADD_DEFINITIONS (-DUSE_FILE32API)
endif()

if (CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(ARCH_DIR "64-bit")
elseif (CMAKE_SIZEOF_VOID_P EQUAL 4)
  set(ARCH_DIR "32-bit")
endif()

if (USE_CHIPMUNK)
    add_definitions(-DCC_ENABLE_CHIPMUNK_INTEGRATION=1)
else()
    add_definitions(-DCC_USE_PHYSICS=0)
endif()

set(PREBUILT_INC)
set(PREBUILT_DIR)

if(ANDROID)
    include_directories(platform/android/jni)

    set(PREBUILT_INC ${PREBUILT_INC}
        ${EXTERNAL_DIR}/freetype2/include/android
        ${EXTERNAL_DIR}/freetype2/include/android/freetype2
        ${EXTERNAL_DIR}/jpeg/include/android
        ${EXTERNAL_DIR}/png/include/android
        ${EXTERNAL_DIR}/zlib/include )

    set(PREBUILT_DIR ${PREBUILT_DIR}
        ${EXTERNAL_DIR}/freetype2/prebuilt/android/${ANDROID_ABI}
        ${EXTERNAL_DIR}/jpeg/prebuilt/android/${ANDROID_ABI}
        ${EXTERNAL_DIR}/png/prebuilt/android/${ANDROID_ABI}
        ${EXTERNAL_DIR}/zlib/prebuilt/android/${ANDROID_ABI}
        )

    if (USE_CHIPMUNK)
        set(PREBUILT_INC ${PREBUILT_INC} ${EXTERNAL_DIR}/chipmunk/include)
        set(PREBUILT_DIR ${PREBUILT_DIR} ${EXTERNAL_DIR}/chipmunk/prebuilt/android/${ANDROID_ABI})
    endif()
elseif(LINUX)
    set(PREBUILT_INC ${PREBUILT_INC}
        ${EXTERNAL_DIR}/freetype2/include/linux
        ${EXTERNAL_DIR}/freetype2/include/linux/freetype
        ${EXTERNAL_DIR}/jpeg/include/linux
        ${EXTERNAL_DIR}/linux-specific/fmod/include )

    set(PREBUILT_DIR ${PREBUILT_DIR}
        ${EXTERNAL_DIR}/freetype2/prebuilt/linux/${ARCH_DIR}
        ${EXTERNAL_DIR}/jpeg/prebuilt/linux/${ARCH_DIR}
        ${EXTERNAL_DIR}/linux-specific/fmod/prebuilt/${ARCH_DIR}
        )

    if (USE_CHIPMUNK)
        set(PREBUILT_INC ${PREBUILT_INC} ${EXTERNAL_DIR}/chipmunk/include)
        set(PREBUILT_DIR ${PREBUILT_DIR} ${EXTERNAL_DIR}/chipmunk/prebuilt/linux/${ARCH_DIR})
    endif()

    include_directories(
        /usr/include
        /usr/include/GLFW
        /usr/local/include/GLFW
        /usr/include/glib-2.0
        /usr/lib/x86_64-linux-gnu/glib-2.0/include
        # gtk-3.0
        /usr/include/gtk-3.0
        /usr/include/at-spi2-atk/2.0
        /usr/include/at-spi-2.0
        /usr/include/dbus-1.0
        /usr/lib/x86_64-linux-gnu/dbus-1.0/include
        /usr/include/gtk-3.0
        /usr/include/gio-unix-2.0/
        /usr/include/cairo
        /usr/include/pango-1.0
        /usr/include/harfbuzz
        /usr/include/pango-1.0
        /usr/include/atk-1.0
        /usr/include/cairo
        /usr/include/pixman-1
        /usr/include/freetype2
        /usr/include/libpng16
        /usr/include/gdk-pixbuf-2.0
    )

endif()

include_directories(${PREBUILT_INC})
link_directories(${PREBUILT_DIR})

