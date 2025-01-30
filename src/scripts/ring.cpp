//=============================================================
//
// リング [ring.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "ring.h"
#include "vehicle.h"
#include "shop/shop.h"
#include "destructible.h"

#include "scene/game.h"

//=============================================================
// [PointRing] 更新
//=============================================================
void PointRing::Update()
{
	// 回転させる
	transform->Rotate(0.0f, ROLL_ANGLE, 0.0f);
}

//=============================================================
// [PointRing] トリガーに入ったとき
//=============================================================
void PointRing::OnTriggerExit(GameObject* other)
{
	if (other->GetComponent<CVehicle>() != nullptr)
	{
		// ポイントを追加する
		ShopManager::AddPoint(POINT);

		// 破棄する
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetDecoration()->RemoveData(gameObject);
		gameObject->Destroy();
	}
}