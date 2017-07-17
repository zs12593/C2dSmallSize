package abc.abc.abc;


import android.os.Handler;

public class HidePay extends PayInterface {

    public HidePay() {
        new Handler().postDelayed(new Runnable() {
            @Override
            public void run() {
                GamePay.nativeSetSDKHide(true);
            }
        }, 500);
    }

    @Override
    public void pay(int payIndex, int money, String product, PayListener listener) {
        if (listener != null)
            listener.payCallback(payIndex, true);
    }

}
