#pragma once
#include <string>

class GamePay {
public:
	static bool isSdkHide();

	static void setSdkHide(bool hide);

	static void buy(int payId);

	static void onBuyCallback(int payId, bool success);

	static std::string getString(int payId);

	static const char* getArg(int index);

};