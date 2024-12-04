//=============================================================
//
// �V���b�v [shop.h]
// Author: Ito Kotaro
// 
// �R�C�����g�p���čw���ł���
//
//=============================================================
#ifndef _SHOP_H_
#define _SHOP_H_

#include "component.h"
#include "component/2d/polygon.h"

// �|�C���g�\��
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

// �V���b�v�Ǘ��N���X
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
