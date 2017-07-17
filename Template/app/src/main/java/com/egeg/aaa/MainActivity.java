package com.egeg.aaa;

import android.os.Bundle;

import org.cocos2dx.lib.Cocos2dxActivity;

import abc.abc.abc.GamePay;

public class MainActivity extends Cocos2dxActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        GamePay.onActivityCreate(this);
    }

    @Override
    protected void onPause() {
        super.onPause();
        GamePay.onActivityPause(this);
    }

    @Override
    protected void onResume() {
        super.onResume();
        GamePay.onActivityResume(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        GamePay.onActivityDestroy(this);
    }

}
