package abc.abc.abc;

public class SuccessPay extends PayInterface {

    @Override
    public void pay(int payIndex, int money, String product, PayListener listener) {
        if (listener != null)
            listener.payCallback(payIndex, true);
    }

}
