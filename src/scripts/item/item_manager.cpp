//=============================================================
//
// アイテム管理 [item_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "item_manager.h"

//=============================================================
// [ItemManager] インベントリにアイテムを追加する
//=============================================================
void ItemManager::AddItem(Item* item)
{
	// アイテムのポインタが被っていないかを確認する
	for (auto itr = m_inventory.begin(); itr != m_inventory.end(); itr++)
	{
		if (*itr == item)
		{ // 被っているとき
			return;
		}
	}

	// インベントリに追加する
	m_inventory.push_back(item);
}

//=============================================================
// [ItemManager] アイテムを削除する
//=============================================================
void ItemManager::RemoveItem(Item* item)
{
	// 持ち込みアイテムに指定のアイテムがある場合、削除する
	for (int i = 0; i < 2; i++)
	{
		if (m_carryOn[i] == item)
		{ // 一致する場合
			m_carryOn[i] = nullptr;
			break;
		}
	}

	// アイテムのポインタが被っていないかを確認する
	for (auto itr = m_inventory.begin(); itr != m_inventory.end(); itr++)
	{
		if (*itr == item)
		{ // 被っているとき
			Item* item = *itr;
			m_inventory.erase(itr);
			delete item;
			break;
		}
	}
}

//=============================================================
// [ItemManager] 持ち運びアイテムを取得する
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
// [ItemManager] 持ち運びアイテムに設定する
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
// [ItemManager] すべてのアイテムを削除する
//=============================================================
void ItemManager::AllRemoveItem()
{
	// 持ち込みアイテムを空にする
	for (int i = 0; i < 2; i++)
	{
		m_carryOn[i] = nullptr;
	}

	// インベントリのアイテムをすべて削除する
	for (auto itr = m_inventory.begin(); itr != m_inventory.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
		}
	}
	m_inventory.clear();
}