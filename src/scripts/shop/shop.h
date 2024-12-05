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

// �|�C���g�\��
class PointView : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;	// ���_���
	LPDIRECT3DTEXTURE9 m_texture;			// �e�N�X�`��
};

// �R�� or �ϋv�l �w�����

// �V���b�v�Ǘ��N���X
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
