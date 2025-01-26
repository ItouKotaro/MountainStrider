//=============================================================
//
// �A�C�e�� [items.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "items.h"

#include "scene/game.h"
#include "scripts/vehicle.h"

//=============================================================
// [FuelTankItem] �g�p
//=============================================================
void FuelTankItem::onUse()
{
	// �Q�[���V�[�����擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �o�C�N���擾����
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	// �R����ǉ�����
	pBike->AddFuel(static_cast<float>(m_amount));

	// �A�C�e���폜
	RemoveItem();
}

//=============================================================
// [ToolBoxItem] �g�p
//=============================================================
void ToolBoxItem::onUse()
{
	// �Q�[���V�[�����擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �o�C�N���擾����
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	// �ϋv�l��ǉ�����
	pBike->AddEndurance(static_cast<float>(m_amount));

	// �A�C�e���폜
	RemoveItem();
}

//=============================================================
// [ExplosionBoostItem] �g�p
//=============================================================
void ExplosionBoostItem::onUse()
{
	// �Q�[���V�[�����擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �o�C�N���擾����
	auto bike = gameScene->GetBike();

	// �ϋv�l��ǉ�����
	CCollision::GetCollision(bike)->GetRigidBody()->applyCentralImpulse(
		btVector3(sinf(bike->transform->GetWRot().y + D3DX_PI) * POWER, sinf(bike->transform->GetWRot().x) * POWER, cosf(bike->transform->GetWRot().y + D3DX_PI) * POWER)
	);

	// �A�C�e���폜
	RemoveItem();
}