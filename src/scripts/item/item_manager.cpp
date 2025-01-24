//=============================================================
//
// �A�C�e���Ǘ� [item_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "item_manager.h"

//=============================================================
// [ItemManager] �C���x���g���ɃA�C�e����ǉ�����
//=============================================================
void ItemManager::AddItem(Item* item)
{
	// �A�C�e���̃|�C���^������Ă��Ȃ������m�F����
	for (auto itr = m_inventory.begin(); itr != m_inventory.end(); itr++)
	{
		if (*itr == item)
		{ // ����Ă���Ƃ�
			return;
		}
	}

	// �C���x���g���ɒǉ�����
	m_inventory.push_back(item);
}

//=============================================================
// [ItemManager] �A�C�e�����폜����
//=============================================================
void ItemManager::RemoveItem(Item* item)
{
	// �������݃A�C�e���Ɏw��̃A�C�e��������ꍇ�A�폜����
	for (int i = 0; i < 2; i++)
	{
		if (m_carryOn[i] == item)
		{ // ��v����ꍇ
			m_carryOn[i] = nullptr;
			break;
		}
	}

	// �A�C�e���̃|�C���^������Ă��Ȃ������m�F����
	for (auto itr = m_inventory.begin(); itr != m_inventory.end(); itr++)
	{
		if (*itr == item)
		{ // ����Ă���Ƃ�
			Item* item = *itr;
			m_inventory.erase(itr);
			delete item;
			break;
		}
	}
}

//=============================================================
// [ItemManager] �����^�уA�C�e�����擾����
//=============================================================
Item* ItemManager::GetCarryOn(int idx)
{
	if (0 <= idx && idx < 2)
	{
		return m_carryOn[idx];
	}
	return nullptr;
}

//=============================================================
// [ItemManager] �����^�уA�C�e���ɐݒ肷��
//=============================================================
bool ItemManager::ToggleCarryOn(Item* item)
{
	if (m_carryOn[0] == item)
	{
		m_carryOn[0] = nullptr;
		return true;
	}
	else if (m_carryOn[1] == item)
	{
		m_carryOn[1] = nullptr;
		return true;
	}
	else
	{
		if (m_carryOn[0] == nullptr)
		{
			m_carryOn[0] = item;
			return true;
		}
		else if (m_carryOn[1] == nullptr)
		{
			m_carryOn[1] = item;
			return true;
		}
		else
		{
			return false;
		}
	}
}

//=============================================================
// [ItemManager] ���ׂẴA�C�e�����폜����
//=============================================================
void ItemManager::AllRemoveItem()
{
	// �������݃A�C�e������ɂ���
	for (int i = 0; i < 2; i++)
	{
		m_carryOn[i] = nullptr;
	}

	// �C���x���g���̃A�C�e�������ׂč폜����
	for (auto itr = m_inventory.begin(); itr != m_inventory.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
		}
	}
	m_inventory.clear();
}