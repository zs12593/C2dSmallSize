
if(LINUX)
    set(COCOS_PLATFORM_SPECIFIC_SRC
      platform/linux/CCStdC-linux.cpp
      platform/linux/CCFileUtils-linux.cpp
      platform/linux/CCCommon-linux.cpp
      platform/linux/CCApplication-linux.cpp
      platform/linux/CCDevice-linux.cpp
      platform/desktop/CCGLViewImpl-desktop.cpp
    )

elseif(ANDROID)

    set(COCOS_PLATFORM_SPECIFIC_SRC
      platform/android/CCApplication-android.cpp
      platform/android/CCCommon-android.cpp
      platform/android/CCGLViewImpl-android.cpp
      platform/android/CCFileUtils-android.cpp
      platform/android/CCEnhanceAPI-android.cpp
      platform/android/jni/JniHelper.cpp
    )

    set(COCOS2DX_ANDROID_STATIC
      platform/android/CCDevice-android.cpp
      platform/android/javaactivity-android.cpp
      platform/android/jni/TouchesJni.cpp
      platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxAccelerometer.cpp
      platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp
      platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp
      platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp

    )

    add_library(cocos2dxandroid_static STATIC ${COCOS2DX_ANDROID_STATIC})

endif()

#leave andatory external stuff here also

include_directories(
  ../external/ConvertUTF
  ../external/edtaa3func
  ../external/poly2tri
  ../external/poly2tri/common
  ../external/poly2tri/sweep
)

set(COCOS_PLATFORM_SRC

  platform/CCSAXParser.cpp
  platform/CCThread.cpp
  platform/CCGLView.cpp
  platform/CCFileUtils.cpp
  platform/CCImage.cpp
  ../external/edtaa3func/edtaa3func.cpp
  ../external/ConvertUTF/ConvertUTFWrapper.cpp
  ../external/ConvertUTF/ConvertUTF.c
  ../external/poly2tri/common/shapes.cc
  ../external/poly2tri/sweep/advancing_front.cc
  ../external/poly2tri/sweep/cdt.cc
  ../external/poly2tri/sweep/sweep_context.cc
  ../external/poly2tri/sweep/sweep.cc
  ${COCOS_PLATFORM_SPECIFIC_SRC}
)
