//=============================================================
//
// �V���b�v�A�C�e�� [shop_items.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shop_items.h"

#include "scene/game.h"
#include "scripts/vehicle.h"

#include "scripts/item/items.h"
#include "scripts/item/item_manager.h"

float ShopItem::m_saleDiscount = 0.0f;
AudioClip ShopItem::m_buySE = nullptr;

//=============================================================
// [ShopItem] ���i�̎擾
//=============================================================
int ShopItem::GetPrice()
{
	return static_cast<int>(m_price * (1.0f - m_discount) * (1.0f - m_saleDiscount));
}

//=============================================================
// [ShopItem] �x����
//=============================================================
bool ShopItem::Pay()
{
	// ��
	if (m_buySE == nullptr) m_buySE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\SHOP\\buy.mp3", FMOD_2D);
	if (m_sePlayer == nullptr)
	{
		m_sePlayer = new GameObject();
		m_sePlayer->AddComponent<AudioSource>();
	}
	m_sePlayer->GetComponent<AudioSource>()->PlayOneShot(m_buySE);

	return m_shopManager->PayPoint(this->GetPrice());
}

//=============================================================
// [FuelItem] �w����
//=============================================================
bool FuelItem::onBuy()
{
	// �Q�[���V�[�����擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �o�C�N���擾����
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	if (pBike->GetFuel() < CVehicle::MAX_FUEL)
	{
		if (this->Pay())
		{ // �x��������
			pBike->AddFuel(static_cast<float>(m_amount));
			return true;
		}
	}
	return false;
}

//=============================================================
// [EnduranceItem] �w����
//=============================================================
bool EnduranceItem::onBuy()
{
	// �Q�[���V�[�����擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �o�C�N���擾����
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	if (pBike->GetEndurance() < CVehicle::MAX_ENDURANCE)
	{
		if (this->Pay())
		{ // �x��������
			pBike->AddEndurance(static_cast<float>(m_amount));
			return true;
		}
	}
	return false;
}

//=============================================================
// [ShopFuelTank] �w����
//=============================================================
bool ShopFuelTank::onBuy()
{
	if (this->Pay())
	{ // �x��������
		// �R���^���N���C���x���g���ɒǉ�
		ItemManager::GetInstance()->AddItem(new FuelTankItem());
		return true;
	}
	return false;
}

//=============================================================
// [ShopToolBox]�w����
//=============================================================
bool ShopToolBox::onBuy()
{
	if (this->Pay())
	{ // �x��������
		// �H����C���x���g���ɒǉ�
		ItemManager::GetInstance()->AddItem(new ToolBoxItem());
		return true;
	}
	return false;
}