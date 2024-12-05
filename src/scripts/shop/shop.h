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

// ポイント表示
class PointView : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;	// 頂点情報
	LPDIRECT3DTEXTURE9 m_texture;			// テクスチャ
};

// 燃料 or 耐久値 購入画面

// ショップ管理クラス
class ShopManager
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVisible(const bool& show) { m_shop->SetVisible(show); }
private:
	GameObject* m_shop;
	GameObject* m_ptView;
};

#endif // !_SHOP_H_
