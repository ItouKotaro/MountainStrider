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