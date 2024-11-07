//=============================================================
//
// 自然の産物 [nature_produces.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "nature_produces.h"
#include "benlib.h"

//=============================================================
// [CNatureProduces] 生成する
//=============================================================
void CNatureProduces::Generate(const Transform& transform)
{
	// パスの入力チェック
	if (m_path.empty()) return;

	// プレハブを生成する
	GameObject* pGenerateObj = GameObject::LoadPrefab(m_path, transform);
	pGenerateObj->AddComponent<CProduceTag>(this);	// 追跡タグ
}

//=============================================================
// [CNatureProduces] 隣接確率を取得する
//=============================================================
float CNatureProduces::GetAdjacentRate(const D3DXVECTOR3& pos)
{
	// 倍率変数
	float _rate = 1.0f;

	// 付近にあるオブジェクトを取得し、倍率を導き出す
	std::vector<CProduceTag*> pAdjacentObj = Component::GetComponents<CProduceTag>();
	for (unsigned int i = 0; i < pAdjacentObj.size(); i++)
	{
		if (Benlib::PosDistance(pos, pAdjacentObj[i]->transform->GetWPos()) <= m_adjacentDistance)
		{ // 範囲内のとき
			_rate *= GetAdjacentObjectRate(typeid(*pAdjacentObj[i]->GetNatureProduce()).name()); // <-ここで判別されない問題あり
		}
	}

	return 1.0f;
}