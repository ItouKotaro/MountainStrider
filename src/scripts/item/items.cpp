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