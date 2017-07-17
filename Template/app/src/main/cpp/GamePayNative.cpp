#include <jni.h>
#include "Pay/GamePay.h"

extern "C" {

JNIEXPORT void JNICALL
Java_abc_abc_abc_GamePay_nativePayCallback(JNIEnv *env, jclass type, jint payIndex,
                                           jboolean success) {
    GamePay::onBuyCallback(payIndex, success);
}

JNIEXPORT void JNICALL
Java_abc_abc_abc_GamePay_nativeSetSDKHide(JNIEnv *env, jclass type, jboolean hide) {
    GamePay::setSdkHide(hide);
}

JNIEXPORT jstring JNICALL
Java_abc_abc_abc_GamePay_nativeGetArg(JNIEnv *env, jclass type, jint index) {
    return env->NewStringUTF(GamePay::getArg(index));
}

}