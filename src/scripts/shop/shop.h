//=============================================================
//
// ショップ [shop.h]
// Author: Ito Kotaro
// 
// コインを使用して購入できる
//
//=============================================================
#ifndef _SHOP_H_
#define _SHOP_H_

// ショップ管理クラス
class ShopManager
{
public:
	void Init();
	void Uninit();
	void Update();
private:
	GameObject* m_text;
};

#endif // !_SHOP_H_
