//=============================================================
//
// アイテム [items.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "items.h"

#include "scene/game.h"
#include "scripts/vehicle.h"

//=============================================================
// [FuelTankItem] 使用
//=============================================================
void FuelTankItem::onUse()
{
	// ゲームシーンを取得する
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// バイクを取得する
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	// 燃料を追加する
	pBike->AddFuel(static_cast<float>(m_amount));

	// アイテム削除
	RemoveItem();
}

//=============================================================
// [ToolBoxItem] 使用
//=============================================================
void ToolBoxItem::onUse()
{
	// ゲームシーンを取得する
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// バイクを取得する
	auto pBike = gameScene->GetBike()->GetComponent<CVehicle>();

	// 耐久値を追加する
	pBike->AddEndurance(static_cast<float>(m_amount));

	// アイテム削除
	RemoveItem();
}

//=============================================================
// [ExplosionBoostItem] 使用
//=============================================================
void ExplosionBoostItem::onUse()
{
	// ゲームシーンを取得する
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// バイクを取得する
	auto bike = gameScene->GetBike();

	// 耐久値を追加する
	CCollision::GetCollision(bike)->GetRigidBody()->applyCentralImpulse(
		btVector3(sinf(bike->transform->GetWRot().y + D3DX_PI) * POWER, sinf(bike->transform->GetWRot().x) * POWER, cosf(bike->transform->GetWRot().y + D3DX_PI) * POWER)
	);

	// アイテム削除
	RemoveItem();
}