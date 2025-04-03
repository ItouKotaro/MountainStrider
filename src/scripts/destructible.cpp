//=============================================================
//
// 破壊可能 [destructible.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "destructible.h"
#include "manager.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/damage_effect.h"
#include "scene/game.h"
#include "vehicle.h"

const float Destructible::FADE_VALUE = 0.03f;
const float Destructible::START_TIME = 3.5f;

//=============================================================
// [Destructible] 初期化
//=============================================================
void Destructible::Init()
{
}

//=============================================================
// [Destructible] 更新
//=============================================================
void Destructible::Update()
{
	if (static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetPause()->GetPause())
		return;

	if (m_isHit)
	{
		// 開始時間が0になるまでは消さない
		if (m_startTimer > 0.0f)
		{
			m_startTimer -= CManager::GetInstance()->GetDeltaTime();
			return;
		}

		// メッシュリストを取得する
		std::vector<GameObject*> pMeshList = gameObject->GetChildren(true);
		for (auto itr = pMeshList.begin(); itr != pMeshList.end(); itr++)
		{
			if (GameObject::IsExist(*itr))
			{
				CMesh* mesh = (*itr)->GetComponent<CMesh>();
				if (mesh != nullptr)
				{
					mesh->SetAlpha(m_fade);
				}
			}
		}

		// フェード処理
		if (m_fade > 0.0f)
		{
			m_fade -= FADE_VALUE;
		}
		else
		{ // 終了
			gameObject->Destroy(true);
			return;
		}
	}
}

//=============================================================
// [Destructible] 衝突
//=============================================================
void Destructible::OnTriggerEnter(GameObject* other)
{
	if (m_isHit) return;

	// バイクコンポーネントを取得する
	CVehicle* vehicle = other->GetComponent<CVehicle>();

	// 衝突処理
	if (vehicle != nullptr || other->GetTag() == "HIT")
	{
		// バイクにダメージを与える
		if (vehicle != nullptr)
		{
			vehicle->AddDamage(m_decoData->type->damage);

			// ダメージエフェクトを出す
			GameObject* damageEffect = new GameObject("DamageEffect", "DMG");
			damageEffect->transform->SetPos(vehicle->transform->GetWPos());
			damageEffect->AddComponent<DamageEffect>()->Generate(m_decoData->type->damage);
		}
		
		// ヒット音を鳴らす
		if (m_decoData->type->hitSound != nullptr)
		{
			gameObject->GetComponent<AudioSource>()->Play(m_decoData->type->hitSound);
			gameObject->GetComponent<AudioSource>()->GetChannel()->setVolume(m_decoData->type->volume);
		}

		// リジッドボディの追加
		CCollision* collision = CCollision::GetCollision(gameObject);
		if (collision != nullptr)
		{
			collision->IsTrigger(false);
			gameObject->AddComponent<CRigidBody>();
		}

		// デコレーションデータから削除する
		if (m_decoData != nullptr)
		{
			m_decoManager->RemoveData(m_decoData);
		}

		// 開始時間を設定する
		m_startTimer = START_TIME;

		m_isHit = true;
	}
}

//=============================================================
// [Destructible] 強制ヒット
//=============================================================
void Destructible::ForceHit(bool playSound)
{
	if (m_isHit) return;

	// ヒット音を鳴らす
	if (m_decoData->type->hitSound != nullptr && playSound)
	{
		gameObject->GetComponent<AudioSource>()->Play(m_decoData->type->hitSound);
		gameObject->GetComponent<AudioSource>()->GetChannel()->setVolume(m_decoData->type->volume);
	}

	// リジッドボディの追加
	CCollision* collision = CCollision::GetCollision(gameObject);
	if (collision != nullptr)
	{
		collision->IsTrigger(false);
		gameObject->AddComponent<CRigidBody>();
	}

	// デコレーションデータから削除する
	if (m_decoData != nullptr)
	{
		m_decoManager->RemoveData(m_decoData);
	}

	// 開始時間を設定する
	m_startTimer = START_TIME;

	m_isHit = true;
}

//=============================================================
// [Destructible] 強制削除
//=============================================================
void Destructible::ForceRemove()
{
	// デコレーションデータから削除する
	if (!m_isHit && m_decoData != nullptr)
	{
		m_decoManager->RemoveData(m_decoData);
	}

	m_startTimer = 0.0f;
	m_fade = 0.0f;
	m_isHit = true;
}