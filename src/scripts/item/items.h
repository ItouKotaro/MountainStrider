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

// �u�[�X�g
class ExplosionBoostItem : public Item
{
public:
	ExplosionBoostItem() : Item("�����u�[�X�g", "�����I�Ƀu�[�X�g����\n�O�֐����悭��������", "data\\TEXTURE\\ITEM\\explosion_boost.png") {}
	void onUse() override;
private:
	const float POWER = 50000.0f;
};

// �Ă�Ă�V��
class TeruTezuBouzuItem : public Item
{
public:
	TeruTezuBouzuItem() : Item("�Ă�Ă�V��", "���ꂪ�K���ƌ����Ă���", "data\\TEXTURE\\ITEM\\teruterubouzu.png") {}
	void onUse() override;
private:

};

#endif // !_ITEMS_H_
