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

class CGameScene;

// �V���b�v�Ǘ��N���X
class ShopManager
{
public:
	void Init();
	void Uninit();
	void Update();
	void Draw();
	void SetVisible(const bool& show) { m_shop->SetVisible(show); }

	static const float BAR_SPACE;
private:
	GameObject* m_shop;		// �y�[�W�Ǘ�
	GameObject* m_ptView;	// ��̃t���[��
	CGameScene* m_gameScene;

	// �R��
	GameObject* m_fuel;
	GameObject* m_fuelBar;
	GameObject* m_fuelCurrentValue;
	GameObject* m_fuelButton;
	GameObject* m_fuelCost;
	GameObject* m_fuelButtonText;

	// �ϋv�l
	GameObject* m_endurance;
	GameObject* m_enduranceBar;
	GameObject* m_enduranceCurrentValue;
	GameObject* m_enduranceButton;
	GameObject* m_enduranceCost;
	GameObject* m_enduranceButtonText;
};

#endif // !_SHOP_H_
