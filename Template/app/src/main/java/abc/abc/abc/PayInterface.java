package abc.abc.abc;

import android.app.Activity;
import android.content.Context;

public abstract class PayInterface {
    public interface PayListener {
        void payCallback(int payIndex, boolean success);
    }

    public abstract void pay(int payIndex,int money, String product, PayListener listener);

    public void onAppAttachContext(Context context) {
    }

    public void onAppCreate(Context context) {
    }

    public void onActivityCreate(Activity activity) {
    }

    public void onActivityPause(Activity activity) {
    }

    public void onActivityResume(Activity activity) {
    }

    public void onActivityDestroy(Activity activity) {
    }
}
