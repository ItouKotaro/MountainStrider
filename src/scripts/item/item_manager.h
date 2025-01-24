//=============================================================
//
// アイテム管理 [item_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

class Item;

// アイテム管理
class ItemManager
{
public:
	// インスタンスの取得
	static ItemManager* GetInstance()
	{
		static ItemManager instance;
		return &instance;
	}

	// すべてのアイテムを削除する（最終処理）
	void AllRemoveItem();

	// アイテムの登録
	void AddItem(Item* item);

	// アイテムの削除
	void RemoveItem(Item* item);

	// インベントリの取得
	std::vector<Item*>* GetInventory() { return &m_inventory; }

	// 持ち込みアイテムの取得
	Item* GetCarryOn(int idx);

	// アイテムの持ち込みに設定する
	bool ToggleCarryOn(Item* item);
private:
	Item* m_carryOn[2];						// 持ち込みアイテム
	std::vector<Item*> m_inventory;	// インベントリ
};

// アイテム（基底）
class Item
{
public:
	Item(const std::string& name, const std::string& description, const std::string& texturePath)
	{
		m_name = name;
		m_description = description;
		m_texturePath = texturePath;
	}
	virtual ~Item() {}

	// 使用時の処理
	virtual void onUse() = 0;

	// アイテム名の取得
	std::string GetName() { return m_name; }

	// 説明の取得
	std::string GetDescription() { return m_description; }

	// テクスチャパスの取得
	std::string GetTexturePath() { return m_texturePath; }
protected:
	void RemoveItem() { ItemManager::GetInstance()->RemoveItem(this); }
private:
	std::string m_name;			// アイテム名
	std::string m_description;	// 説明
	std::string m_texturePath;	// テクスチャパス
};

#endif // !_ITEM_MANAGER_H_
