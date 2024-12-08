//=============================================================
//
// �V���b�v�A�C�e�� [shop_items.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shop_items.h"

#include "scene/game.h"
#include "scripts/vehicle.h"

float ShopItem::m_saleDiscount = 0.0f;

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
// [FuelTankItem] �w����
//=============================================================
bool FuelTankItem::onBuy()
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
// [ToolBoxItem] �w����
//=============================================================
bool ToolBoxItem::onBuy()
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