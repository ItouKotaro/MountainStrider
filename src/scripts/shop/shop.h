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
#include "shop_items.h"

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
	static void AddPoint(const int& point) { m_points += point; }
	static void Reset();

	// 支払えた時にtrueを返し、失敗すると消費されずにfalseを返します
	bool PayPoint(const int& cost);

	// 所持金を取得する
	int GetPoints() { return m_points; }

	static const float BAR_SPACE;
private:
	void InitTopDisplay();			// 燃料と耐久値の購入画面の初期化
	void RegisterItemList();		// アイテムの登録

	static int m_points;				// ポイント
	int m_viewPoints;				// 表示されているポイント
	int m_pointsCounter;			// ポイントを更新するカウンター

	GameObject* m_shop;		// ページ管理
	GameObject* m_ptView;	// 上のフレーム
	GameObject* m_ptText;		// ポイントテキスト
	CGameScene* m_gameScene;

	// ショップアイテムの管理
	FuelItem m_fuelItem;									// 燃料アイテム
	EnduranceItem m_enduranceItem;				// 耐久値アイテム
	GameObject* m_shopItems[6];					// ショップアイテム
	std::vector<ShopItem*> m_itemList;			// アイテムリスト
	std::vector<ShopItem*> m_perkList;			// パークリスト

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
