package abc.abc.abc;

import android.app.Activity;
import android.content.Context;
import android.text.TextUtils;
import android.util.Log;

import java.io.DataInputStream;
import java.io.IOException;
import java.io.InputStream;

public class GamePay {
    private static PayInterface payImpl;
    private static PayInterface.PayListener payListener = new PayInterface.PayListener() {
        @Override
        public void payCallback(int payIndex, boolean success) {
            nativePayCallback(payIndex, success);
        }
    };

    public static void onAppAttachContext(Context context) {
        Log.v("GamePay", "==== onAppAttachContext");
        InputStream in = null;
        String sdkName = null;
        try {
            in = context.getAssets().open("Config.bin");
            DataInputStream dis = new DataInputStream(in);
            dis.readInt();
            dis.readUTF();
            sdkName = dis.readUTF();
        } catch (Exception e) {
            e.printStackTrace();
        } finally {
            if (in != null)
                try {
                    in.close();
                } catch (IOException e) {
                    e.printStackTrace();
                }
        }
        try {
            if (TextUtils.isEmpty(sdkName))
                sdkName = "Success";
            Log.v("GamePay", "==== sdkName: " + sdkName);
            Class clz = Class.forName("abc.abc.abc." + sdkName + "Pay");
            payImpl = (PayInterface) clz.newInstance();
            payImpl.onAppAttachContext(context);
        } catch (Exception e) {
            Log.v("GamePay", "==== sdkName: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public static void onAppCreate(Context context) {
        payImpl.onAppCreate(context);
    }

    public static void onActivityCreate(Activity act) {
        payImpl.onActivityCreate(act);
    }

    public static void onActivityPause(Activity activity) {
        payImpl.onActivityPause(activity);
    }

    public static void onActivityResume(Activity activity) {
        payImpl.onActivityResume(activity);
    }

    public static void onActivityDestroy(Activity activity) {
        payImpl.onActivityDestroy(activity);
    }

    public static void pay(int payIndex, int money, String product) {
        Log.v("GamePay", "==== Java GamePay payIndex: " + payIndex);
        payImpl.pay(payIndex, money, product, payListener);
    }

    private static native void nativePayCallback(int payIndex, boolean success);

    public static native void nativeSetSDKHide(boolean hide);

    public static native String nativeGetArg(int index);

}
