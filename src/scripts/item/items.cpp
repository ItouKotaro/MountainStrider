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

//=============================================================
// [TeruTezuBouzuItem] �g�p
//=============================================================
void TeruTezuBouzuItem::onUse()
{
	// �Q�[���V�[�����擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// ����ɂ���
	gameScene->GetEnvironmental()->SetWeather(new WeatherClear());

	// �A�C�e���폜
	RemoveItem();
}

//=============================================================
// [MyRoadItem] �g�p
//=============================================================
void MyRoadItem::onUse()
{
	// �Q�[���V�[�����擾����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �o�C�N���擾����
	auto bike = gameScene->GetBike();

	// �o�C�N�̈ʒu�ƕ������瓹��ݒu����ʒu����ݒ肷��
	Transform roadTrans;
	roadTrans.SetPos(
		D3DXVECTOR3(sinf(bike->transform->GetWRot().y + D3DX_PI) * FRONT_LENGTH, SET_HEIGHT, cosf(bike->transform->GetWRot().y + D3DX_PI) * FRONT_LENGTH) +
		bike->transform->GetWPos()
	);
	roadTrans.SetRot(0.0f, bike->transform->GetWRot().y, 0.0f);
	
	// ��������
	GameObject::LoadPrefab("data\\PREFAB\\myroad.pref", roadTrans);

	// �A�C�e���폜
	RemoveItem();
}