//=============================================================
//
// �A�C�e�� [items.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _ITEMS_H_
#define _ITEMS_H_

#include "item_manager.h"

// �R���^���N
class FuelTankItem : public Item
{
public:
	FuelTankItem() : Item("�R���^���N", "�R����+300�񕜂���\n�����ɂ͕K�{�ȃA�C�e��", "data\\TEXTURE\\ITEM\\fuel_tank.png"), m_amount(300) {}
	void onUse() override;
private:
	int m_amount;
};

// �R���^���N
class ToolBoxItem : public Item
{
public:
	ToolBoxItem() : Item("�H�", "�ϋv�l��+100�񕜂���\n�悭�Փ˂���l�ɂ͕K�{�ȃA�C�e��", "data\\TEXTURE\\ITEM\\toolbox.png"), m_amount(100) {}
	void onUse() override;
private:
	int m_amount;
};

#endif // !_ITEMS_H_
