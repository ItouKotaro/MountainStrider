//=============================================================
//
// 自然の産物 [nature_produces.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "nature_produces.h"

//=============================================================
// [CNatureProduces] 生成する
//=============================================================
void CNatureProduces::Generate(const Transform& transform)
{
	// パスの入力チェック
	if (m_path.empty()) return;

	// プレハブを生成する
	GameObject* pGenerateObj = GameObject::LoadPrefab(m_path, transform);
	pGenerateObj->AddComponent<CProduceTag>(typeid(*this).name());	// 追跡タグ
}

//=============================================================
// [CNatureProduces] 隣接確率を取得する
//=============================================================
float CNatureProduces::GetAdjacentRate(const D3DXVECTOR3& pos)
{
	// 
	return 1.0f;
}