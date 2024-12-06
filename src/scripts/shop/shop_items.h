//=============================================================
//
// ショップアイテム [shop_items.h]
// Author: Ito Kotaro
// 
// ショップのアイテムを管理します
// 値段設定など
//
//=============================================================
#ifndef _SHOP_ITEMS_H_
#define _SHOP_ITEMS_H_

class ShopManager;

// ショップアイテム（基底）
class ShopItem
{
public:
	ShopItem(const int& price) : m_discount(0.0f), m_shopManager(nullptr) { m_price = price; }
	virtual ~ShopItem() {}

	// ショップマネージャーを設定する
	void SetShopManager(ShopManager* shopManager) { m_shopManager = shopManager; }

	// 購入時の処理
	virtual void onBuy() = 0;

	// 価格を取得する
	int GetPrice();

	// 割引率を設定する
	void SetDiscount(const float& discount) { m_discount = discount; }

	// セール割引率を設定する
	static void SetSaleDiscount(const float& discount) { m_saleDiscount = discount; }
protected:
	// 支払い
	bool Pay();

	ShopManager* m_shopManager;		// ショップマネージャー

	int m_price;							// 価格
	float m_discount;					// 割引率
	static float m_saleDiscount;		// セール割引率
};

// 燃料
class FuelItem : public ShopItem
{
public:
	FuelItem() : ShopItem(5), m_amount(300) {}
	void onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};

// 耐久値
class EnduranceItem : public ShopItem
{
public:
	EnduranceItem() : ShopItem(5), m_amount(50) {}
	void onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};

#endif // !_SHOP_ITEMS_H_
