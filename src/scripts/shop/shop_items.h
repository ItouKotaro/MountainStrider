//=============================================================
//
// �V���b�v�A�C�e�� [shop_items.h]
// Author: Ito Kotaro
// 
// �V���b�v�̃A�C�e�����Ǘ����܂�
// �l�i�ݒ�Ȃ�
//
//=============================================================
#ifndef _SHOP_ITEMS_H_
#define _SHOP_ITEMS_H_

class ShopManager;

// �V���b�v�A�C�e���i���j
class ShopItem
{
public:
	// �A�C�e���̎��
	enum TYPE
	{
		ITEM,
		PERK,
	};

	ShopItem(const TYPE& type, const int& price, const std::string& name, const std::string& description, const std::string& texturePath, const std::string& detail = "") : m_discount(0.0f), m_shopManager(nullptr)
	{
		m_itemType = type;
		m_itemName = name;
		m_itemDescription = description;
		m_detail = detail;
		m_texturePath = texturePath;
		m_price = price;
	}
	virtual ~ShopItem() { m_sePlayer->Destroy(); }

	// �V���b�v�}�l�[�W���[��ݒ肷��
	void SetShopManager(ShopManager* shopManager) { m_shopManager = shopManager; }

	// �w�����̏���
	virtual bool onBuy() = 0;

	// ���i��ݒ肷��
	void SetPrice(const int& price) { m_price = price; }

	// ���i���擾����
	int GetPrice();

	// ��������ݒ肷��
	void SetDiscount(const float& discount) { m_discount = discount; }

	// �Z�[����������ݒ肷��
	static void SetSaleDiscount(const float& discount) { m_saleDiscount = discount; }

	// �A�C�e�������擾����
	std::string GetName() { return m_itemName; }

	// �������擾����
	std::string GetDescription() { return m_itemDescription; }

	// �ڍא������擾����
	std::string GetDetail() { return m_detail; }

	// �e�N�X�`���p�X���擾����
	std::string GetTexturePath() { return m_texturePath; }

	// �V���b�v�}�l�[�W���[���擾����
	ShopManager* GetShopManager() { return m_shopManager; }

	// �A�C�e���̎�ނ��擾����
	TYPE GetType() { return m_itemType; }
protected:
	// �x����
	bool Pay();

	ShopManager* m_shopManager;		// �V���b�v�}�l�[�W���[

	// �A�C�e�����
	std::string m_itemName;			// ���O
	std::string m_itemDescription;	// ����
	std::string m_texturePath;			// �e�N�X�`���p�X
	std::string m_detail;					// �ڍא���
	TYPE m_itemType;						// �A�C�e���̎��

	// ���i���
	int m_price;							// ���i
	float m_discount;					// ������
	static float m_saleDiscount;		// �Z�[��������

	// �T�E���h
	GameObject* m_sePlayer;
	static AudioClip m_buySE;
};

// �R��
class FuelItem : public ShopItem
{
public:
	FuelItem() : ShopItem(TYPE::ITEM, 5, "�R��", "�R�����񕜂��܂�", ""), m_amount(300) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// ������
};

// �ϋv�l
class EnduranceItem : public ShopItem
{
public:
	EnduranceItem() : ShopItem(TYPE::ITEM, 5, "�ϋv�l", "�ϋv�l���񕜂��܂�", ""), m_amount(50) {}
	bool onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// ������
};

//----------------------------------------------------------------------------------------------------------------------------

// �R���^���N
class ShopFuelTank : public ShopItem
{
public:
	ShopFuelTank() : ShopItem(TYPE::ITEM, 8, "�R���^���N", "�R���𑦎��񕜂��܂�", "data\\TEXTURE\\ITEM\\fuel_tank.png", "�R����+300�񕜂��܂�\n����肵�����l�ɂ�������") {}
	bool onBuy() override;
};

// �H�
class ShopToolBox : public ShopItem
{
public:
	ShopToolBox() : ShopItem(TYPE::ITEM, 8, "�H�", "�ϋv�l�𑦎��񕜂��܂�", "data\\TEXTURE\\ITEM\\toolbox.png", "�ϋv�l��+100�񕜂��܂�\n�悭�Փˎ��̂��N�����l�ɂ͕K�{�A�C�e��") {}
	bool onBuy() override;
};

// �����u�[�X�g
class ShopExplosionBoost : public ShopItem
{
public:
	ShopExplosionBoost() : ShopItem(TYPE::ITEM, 15, "�����u�[�X�g", "�O���֒��������܂�", "data\\TEXTURE\\ITEM\\explosion_boost.png", "�����I�Ƀu�[�X�g����\n�O�֐����悭��������") {}
	bool onBuy() override;
};

// �Ă�Ă�V��
class ShopTeruTeruBouzu : public ShopItem
{
public:
	ShopTeruTeruBouzu() : ShopItem(TYPE::ITEM, 10, "�Ă�Ă�V��", "���ꂪ�K���H", "data\\TEXTURE\\ITEM\\teruterubouzu.png", "���ꂪ�K���ƌ����Ă���\n���V�󎞂Ɏg���Ă݂��...") {}
	bool onBuy() override;
};

// �}�C���[�h
class ShopMyRoad : public ShopItem
{
public:
	ShopMyRoad() : ShopItem(TYPE::ITEM, 10, "�}�C���[�h", "�䂪�����o������", "data\\TEXTURE\\ITEM\\myroad.png", "�ڂ̑O�ɓ��������\n�t�̂ŔG�ꂽ���Ȃ��l��") {}
	bool onBuy() override;
};


#endif // !_SHOP_ITEMS_H_
