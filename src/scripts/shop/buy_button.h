//=============================================================
//
// 購入ボタン [buy_button.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _BUY_BUTTON_H_
#define _BUY_BUTTON_H_

#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "shop_items.h"

// 購入ボタン
class BuyButtonUI : public Component
{
public:
	BuyButtonUI(ShopItem* shopItem) { m_shopItem = shopItem; }
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	static const float EDGE_BOLD;		// 縁の太さ
	static const float WIDTH;
	static const float HEIGHT;
	static const float ITEM_SIZE;
	static const float PRICEICON_SIZE;
	static const D3DXCOLOR ITEM_COLOR;
	static const D3DXCOLOR ITEM_COLOR_BOUGHT;
	static const D3DXCOLOR PERK_COLOR;
	static const D3DXCOLOR PERK_COLOR_BOUGHT;
private:
	ShopItem* m_shopItem;
	float m_progress;

	SingleComponent<CPolygon>* m_edge;			// 縁
	SingleComponent<CPolygon>* m_bought;		// 購入後
	SingleComponent<CPolygon>* m_inside;		// 購入前
	SingleComponent<CPolygon>* m_mask;			// マスク（進行表示用）
	SingleComponent<CPolygon>* m_image;		// イメージ画像
	SingleComponent<CText>* m_itemName;		// アイテム名
	SingleComponent<CPolygon>* m_priceIcon	;	// 価格アイコン
	SingleComponent<CText>* m_priceText;			// 価格テキスト
	SingleComponent<CPolygon>* m_splitLine;		// 仕分け線
	SingleComponent<CText>* m_description;		// 説明
};

#endif // !_BUY_BUTTON_H_
