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
	// アイテムの種類
	enum TYPE
	{
		ITEM,
		PERK,
	};

	ShopItem(const TYPE& type, const std::string& name, const std::string& description, const std::string& texturePath, const int& price) : m_discount(0.0f), m_shopManager(nullptr)
	{
		m_itemType = type;
		m_itemName = name;
		m_itemDescription = description;
		m_texturePath = texturePath;
		m_price = price;
	}
	virtual ~ShopItem() {}

	// ショップマネージャーを設定する
	void SetShopManager(ShopManager* shopManager) { m_shopManager = shopManager; }

	// 購入時の処理
	virtual bool onBuy() = 0;

	// 価格を取得する
	int GetPrice();

	// 割引率を設定する
	void SetDiscount(const float& discount) { m_discount = discount; }

	// セール割引率を設定する
	static void SetSaleDiscount(const float& discount) { m_saleDiscount = discount; }

	// アイテム名を取得する
	std::string GetName() { return m_itemName; }

	// 説明を取得する
	std::string GetDescription() { return m_itemDescription; }

	// テクスチャパスを取得する
	std::string GetTexturePath() { return m_texturePath; }

	// アイテムの種類を取得する
	TYPE GetType() { return m_itemType; }
protected:
	// 支払い
	bool Pay();

	ShopManager* m_shopManager;		// ショップマネージャー

	// アイテム情報
	std::string m_itemName;			// 名前
	std::string m_itemDescription;	// 説明
	std::string m_texturePath;			// テクスチャパス
	TYPE m_itemType;						// アイテムの種類

	// 価格情報
	int m_price;							// 価格
	float m_discount;					// 割引率
	static float m_saleDiscount;		// セール割引率
};

// 燃料
class FuelItem : public ShopItem
{
public:
	FuelItem() : ShopItem(TYPE::ITEM, "燃料", "燃料を回復します", "", 5), m_amount(300) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};

// 耐久値
class EnduranceItem : public ShopItem
{
public:
	EnduranceItem() : ShopItem(TYPE::ITEM, "耐久値", "耐久値を回復します", "", 5), m_amount(50) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};

//----------------------------------------------------------------------------------------------------------------------------

// 燃料タンク
class FuelTankItem : public ShopItem
{
public:
	FuelTankItem() : ShopItem(TYPE::ITEM, "燃料タンク", "燃料を即時回復します", "data\\TEXTURE\\ITEM\\fuel_tank.png", 5), m_amount(300) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};

// 工具箱
class ToolBoxItem : public ShopItem
{
public:
	ToolBoxItem() : ShopItem(TYPE::ITEM, "工具箱", "耐久値を即時回復します", "data\\TEXTURE\\ITEM\\toolbox.png", 5), m_amount(100) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};


#endif // !_SHOP_ITEMS_H_
