//=============================================================
//
// �A�C�e���Ǘ� [item_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ITEM_MANAGER_H_
#define _ITEM_MANAGER_H_

class Item;

// �A�C�e���Ǘ�
class ItemManager
{
public:
	// �C���X�^���X�̎擾
	static ItemManager* GetInstance()
	{
		static ItemManager instance;
		return &instance;
	}

	// ���ׂẴA�C�e�����폜����i�ŏI�����j
	void AllRemoveItem();

	// �A�C�e���̓o�^
	void AddItem(Item* item);

	// �A�C�e���̍폜
	void RemoveItem(Item* item);

	// �C���x���g���̎擾
	std::vector<Item*>* GetInventory() { return &m_inventory; }

	// �������݃A�C�e���̎擾
	Item* GetCarryOn(int idx);

	// �A�C�e���̎������݂ɐݒ肷��
	void ToggleCarryOn(Item* item);
private:
	Item* m_carryOn[2];						// �������݃A�C�e��
	std::vector<Item*> m_inventory;	// �C���x���g��
};

// �A�C�e���i���j
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

	// �g�p���̏���
	virtual void onUse() = 0;

	// �A�C�e�����̎擾
	std::string GetName() { return m_name; }

	// �����̎擾
	std::string GetDescription() { return m_description; }

	// �e�N�X�`���p�X�̎擾
	std::string GetTexturePath() { return m_texturePath; }
protected:
	void RemoveItem() { ItemManager::GetInstance()->RemoveItem(this); }
private:
	std::string m_name;			// �A�C�e����
	std::string m_description;	// ����
	std::string m_texturePath;	// �e�N�X�`���p�X
};

#endif // !_ITEM_MANAGER_H_
