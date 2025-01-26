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

	ShopItem(const TYPE& type, const int& price, const std::string& name, const std::string& description, const std::string& texturePath, const std::string& detail = "") : m_discount(0.0f), m_shopManager(nullptr)
	{
		m_itemType = type;
		m_itemName = name;
		m_itemDescription = description;
		m_detail = detail;
		m_texturePath = texturePath;
		m_price = price;
	}
	virtual ~ShopItem() { m_sePlayer->Destroy(); }

	// ショップマネージャーを設定する
	void SetShopManager(ShopManager* shopManager) { m_shopManager = shopManager; }

	// 購入時の処理
	virtual bool onBuy() = 0;

	// 価格を設定する
	void SetPrice(const int& price) { m_price = price; }

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

	// 詳細説明を取得する
	std::string GetDetail() { return m_detail; }

	// テクスチャパスを取得する
	std::string GetTexturePath() { return m_texturePath; }

	// ショップマネージャーを取得する
	ShopManager* GetShopManager() { return m_shopManager; }

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
	std::string m_detail;					// 詳細説明
	TYPE m_itemType;						// アイテムの種類

	// 価格情報
	int m_price;							// 価格
	float m_discount;					// 割引率
	static float m_saleDiscount;		// セール割引率

	// サウンド
	GameObject* m_sePlayer;
	static AudioClip m_buySE;
};

// 燃料
class FuelItem : public ShopItem
{
public:
	FuelItem() : ShopItem(TYPE::ITEM, 5, "燃料", "燃料を回復します", ""), m_amount(300) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};

// 耐久値
class EnduranceItem : public ShopItem
{
public:
	EnduranceItem() : ShopItem(TYPE::ITEM, 5, "耐久値", "耐久値を回復します", ""), m_amount(50) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// 増加量
};

//----------------------------------------------------------------------------------------------------------------------------

// 燃料タンク
class ShopFuelTank : public ShopItem
{
public:
	ShopFuelTank() : ShopItem(TYPE::ITEM, 8, "燃料タンク", "燃料を即時回復します", "data\\TEXTURE\\ITEM\\fuel_tank.png", "燃料が+300回復します\n遠回りしたい人におすすめ") {}
	bool onBuy() override;
};

// 工具箱
class ShopToolBox : public ShopItem
{
public:
	ShopToolBox() : ShopItem(TYPE::ITEM, 8, "工具箱", "耐久値を即時回復します", "data\\TEXTURE\\ITEM\\toolbox.png", "耐久値が+100回復します\nよく衝突事故を起こす人には必須アイテム") {}
	bool onBuy() override;
};

// 爆発ブースト
class ShopExplosionBoost : public ShopItem
{
public:
	ShopExplosionBoost() : ShopItem(TYPE::ITEM, 15, "爆発ブースト", "前方へ超加速します", "data\\TEXTURE\\ITEM\\explosion_boost.png", "爆発的にブーストする\n前へ勢いよく加速する") {}
	bool onBuy() override;
};

// てるてる坊主
class ShopTeruTeruBouzu : public ShopItem
{
public:
	ShopTeruTeruBouzu() : ShopItem(TYPE::ITEM, 10, "てるてる坊主", "晴れが訪れる？", "data\\TEXTURE\\ITEM\\teruterubouzu.png", "晴れが訪れると言われている\n悪天候時に使ってみると...") {}
	bool onBuy() override;
};

// マイロード
class ShopMyRoad : public ShopItem
{
public:
	ShopMyRoad() : ShopItem(TYPE::ITEM, 10, "マイロード", "我が道が出現する", "data\\TEXTURE\\ITEM\\myroad.png", "目の前に道が現れる\n液体で濡れたくない人に") {}
	bool onBuy() override;
};


#endif // !_SHOP_ITEMS_H_
