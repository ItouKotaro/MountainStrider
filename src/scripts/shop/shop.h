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
#include "shop_items.h"

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
	static void AddPoint(const int& point) { m_points += point; }
	static void Reset();

	// �x����������true��Ԃ��A���s����Ə���ꂸ��false��Ԃ��܂�
	bool PayPoint(const int& cost);

	// ���������擾����
	int GetPoints() { return m_points; }

	static const float BAR_SPACE;
private:
	void InitTopDisplay();			// �R���Ƒϋv�l�̍w����ʂ̏�����
	void RegisterItemList();		// �A�C�e���̓o�^

	static int m_points;				// �|�C���g
	int m_viewPoints;				// �\������Ă���|�C���g
	int m_pointsCounter;			// �|�C���g���X�V����J�E���^�[

	GameObject* m_shop;		// �y�[�W�Ǘ�
	GameObject* m_ptView;	// ��̃t���[��
	GameObject* m_ptText;		// �|�C���g�e�L�X�g
	CGameScene* m_gameScene;

	// �V���b�v�A�C�e���̊Ǘ�
	FuelItem m_fuelItem;									// �R���A�C�e��
	EnduranceItem m_enduranceItem;				// �ϋv�l�A�C�e��
	GameObject* m_shopItems[6];					// �V���b�v�A�C�e��
	std::vector<ShopItem*> m_itemList;			// �A�C�e�����X�g
	std::vector<ShopItem*> m_perkList;			// �p�[�N���X�g

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
