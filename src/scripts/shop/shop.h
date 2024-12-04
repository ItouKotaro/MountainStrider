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
#include "component/2d/polygon.h"

// ポイント表示
class PointView : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	CPolygon* m_pointFrame;
};

// ショップ管理クラス
class ShopManager
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
private:
	GameObject* m_text;
	PointView* m_ptView;
};

#endif // !_SHOP_H_
