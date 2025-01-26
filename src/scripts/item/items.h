//=============================================================
//
// アイテム [items.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "item_manager.h"

// 燃料タンク
class FuelTankItem : public Item
{
public:
	FuelTankItem() : Item("燃料タンク", "燃料が+300回復する\n遠回りには必須なアイテム", "data\\TEXTURE\\ITEM\\fuel_tank.png"), m_amount(300) {}
	void onUse() override;
private:
	int m_amount;
};

// 燃料タンク
class ToolBoxItem : public Item
{
public:
	ToolBoxItem() : Item("工具箱", "耐久値が+100回復する\nよく衝突する人には必須なアイテム", "data\\TEXTURE\\ITEM\\toolbox.png"), m_amount(100) {}
	void onUse() override;
private:
	int m_amount;
};

// ブースト
class ExplosionBoostItem : public Item
{
public:
	ExplosionBoostItem() : Item("爆発ブースト", "爆発的にブーストする\n前へ勢いよく加速する", "data\\TEXTURE\\ITEM\\explosion_boost.png") {}
	void onUse() override;
private:
	const float POWER = 50000.0f;
};

// てるてる坊主
class TeruTezuBouzuItem : public Item
{
public:
	TeruTezuBouzuItem() : Item("てるてる坊主", "晴れが訪れると言われている", "data\\TEXTURE\\ITEM\\teruterubouzu.png") {}
	void onUse() override;
private:
};

// マイロード
class MyRoadItem : public Item
{
public:
	MyRoadItem() : Item("マイロード", "我が道が出現する", "data\\TEXTURE\\ITEM\\myroad.png") {}
	void onUse() override;
private:
	const float FRONT_LENGTH = 180.0f;
	const float SET_HEIGHT = -5.0f;
};

#endif // !_ITEMS_H_
