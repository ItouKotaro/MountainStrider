//=============================================================
//
// リング [ring.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "ring.h"
#include "vehicle.h"
#include "shop/shop.h"
#include "wreckage.h"

#include "scene/game.h"

AudioClip PointRing::m_throughSE = nullptr;

//=============================================================
// [PointRing] 更新
//=============================================================
void PointRing::Update()
{
	if (static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetPause()->GetPause())
		return;

	// 回転させる
	transform->Rotate(0.0f, ROLL_ANGLE, 0.0f);
}

//=============================================================
// [PointRing] トリガーに入ったとき
//=============================================================
void PointRing::OnTriggerExit(GameObject* other)
{
	if (!m_isUsed && other->GetComponent<CVehicle>() != nullptr)
	{
		// 音を生成する
		if (!m_throughSE) m_throughSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\HIT\\ring.mp3");

		// ポイントを追加する
		ShopManager::AddPoint(POINT);

		// リストから削除する
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetDecoration()->RemoveData(gameObject);
		
		// 徐々に破棄する
		gameObject->AddComponent<CWreckage>();

		// 音を鳴らす
		gameObject->AddComponent<AudioSource>()->Play(m_throughSE);

		m_isUsed = true;
	}
}