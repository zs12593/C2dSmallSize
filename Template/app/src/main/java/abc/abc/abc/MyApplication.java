package abc.abc.abc;

import android.app.Application;
import android.content.Context;

public class MyApplication extends Application {

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);
        GamePay.onAppAttachContext(base);
    }

    @Override
    public void onCreate() {
        super.onCreate();
        GamePay.onAppCreate(this);
    }

}
