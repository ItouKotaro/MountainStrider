//=============================================================
//
// 購入ボタン [buy_button.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _BUY_BUTTON_H_
#define _BUY_BUTTON_H_

#include "component/other/button.h"

// 購入ボタン
class BuyButtonUI : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	
};

#endif // !_BUY_BUTTON_H_
