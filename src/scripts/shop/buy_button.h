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
	BuyButtonUI() : m_vtxBuff(nullptr), m_texture(nullptr) {}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;	// 頂点情報
	LPDIRECT3DTEXTURE9 m_texture;				// テクスチャ
};

#endif // !_BUY_BUTTON_H_
