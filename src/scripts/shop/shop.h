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

// �V���b�v�Ǘ��N���X
class ShopManager
{
public:
	void Init();
	void Uninit();
	void Update();
private:
	GameObject* m_text;
};

#endif // !_SHOP_H_
