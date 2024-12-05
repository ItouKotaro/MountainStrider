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

#include "component.h"

class CGameScene;

// ショップ管理クラス
class ShopManager
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVisible(const bool& show) { m_shop->SetVisible(show); }

	static const float BAR_SPACE;
private:
	GameObject* m_shop;		// ページ管理
	GameObject* m_ptView;	// 上のフレーム
	CGameScene* m_gameScene;

	// 燃料
	GameObject* m_fuel;
	GameObject* m_fuelBar;
	GameObject* m_fuelCurrentValue;
	GameObject* m_fuelButton;
	GameObject* m_fuelCost;
	GameObject* m_fuelButtonText;

	// 耐久値
	GameObject* m_endurance;
	GameObject* m_enduranceBar;
	GameObject* m_enduranceCurrentValue;
	GameObject* m_enduranceButton;
	GameObject* m_enduranceCost;
	GameObject* m_enduranceButtonText;
};

#endif // !_SHOP_H_
