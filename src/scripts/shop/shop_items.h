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
	ShopItem(const int& price) : m_discount(0.0f), m_shopManager(nullptr) { m_price = price; }
	virtual ~ShopItem() {}

	// �V���b�v�}�l�[�W���[��ݒ肷��
	void SetShopManager(ShopManager* shopManager) { m_shopManager = shopManager; }

	// �w�����̏���
	virtual void onBuy() = 0;

	// ���i���擾����
	int GetPrice();

	// ��������ݒ肷��
	void SetDiscount(const float& discount) { m_discount = discount; }

	// �Z�[����������ݒ肷��
	static void SetSaleDiscount(const float& discount) { m_saleDiscount = discount; }
protected:
	// �x����
	bool Pay();

	ShopManager* m_shopManager;		// �V���b�v�}�l�[�W���[

	int m_price;							// ���i
	float m_discount;					// ������
	static float m_saleDiscount;		// �Z�[��������
};

// �R��
class FuelItem : public ShopItem
{
public:
	FuelItem() : ShopItem(5), m_amount(300) {}
	void onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// ������
};

// �ϋv�l
class EnduranceItem : public ShopItem
{
public:
	EnduranceItem() : ShopItem(5), m_amount(50) {}
	void onBuy() override;
	int GetAmount() { return m_amount; }
private:
	int m_amount;	// ������
};

#endif // !_SHOP_ITEMS_H_
