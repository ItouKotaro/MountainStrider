//=============================================================
//
// 自然の産物 [nature_produces.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "nature_produces.h"
#include "benlib.h"
#include "scene/game.h"

//=============================================================
// [CNatureProduces] 生成する
//=============================================================
GameObject* CNatureProduces::Generate(const Transform& transform)
{
	// パスの入力チェック
	if (m_path.empty()) return nullptr;

	// プレハブを生成する
	GameObject* pGenerateObj = GameObject::LoadPrefab(m_path, transform);

	return pGenerateObj;
}

//=============================================================
// [CNatureProduces] 隣接確率を取得する
//=============================================================
float CNatureProduces::GetAdjacentRate(const D3DXVECTOR3& pos)
{
	// 地形の取得
	auto pProducesManager = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain()->GetProducesManager();
	
	float rate = 1.0f;

#ifndef _DEBUG
	rate = pProducesManager->GetNearProducesRate(m_produceName, pos, m_adjacentDistance);
#endif // _DEBUG
			
	return rate;
}