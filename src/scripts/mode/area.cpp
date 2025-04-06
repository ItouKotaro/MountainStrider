//=============================================================
//
// エリアモード [area.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "area.h"
#include "manager.h"

//=============================================================
// [AreaMode] 初期化
//=============================================================
void AreaMode::Init()
{
	m_point = 0;
	m_nextArea = 0.0f;

	// 地形を生成する
	m_game->GenerateTerrain(150, 150.0f);
}

//=============================================================
// [AreaMode] 終了
//=============================================================
void AreaMode::Uninit()
{
}

//=============================================================
// [AreaMode] 更新
//=============================================================
void AreaMode::Update()
{
	// エリアの生成
	m_nextArea -= CManager::GetInstance()->GetDeltaTime();
	if (m_nextArea <= 0.0f)
	{
		// エリアの生成
		float distanceHalf = (m_game->GetTerrain()->GetTerrainSize() * m_game->GetTerrain()->GetTerrainScale()) / 2.0f;
		GameObject* nextArea = new GameObject("Area");
		nextArea->transform->SetPos(RandomFloat(-distanceHalf, distanceHalf), 0.0f, RandomFloat(-distanceHalf, distanceHalf));
		nextArea->AddComponent<AreaPoint>();

		// 次のエリア
		m_nextArea = RandomFloat(10.0f, 20.0f);
	}
}



//=============================================================
// [AreaPoint] 初期化
//=============================================================
void AreaPoint::Init()
{
	// バイクを検索する
	m_vehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [AreaPoint] 終了
//=============================================================
void AreaPoint::Uninit()
{
}

//=============================================================
// [AreaPoint] 更新
//=============================================================
void AreaPoint::Update()
{
	if (Benlib::PosPlaneDistance(m_vehicle->transform->GetWPos(), transform->GetWPos()) < 300.0f)
	{
		// ポイントを加算する
		static_cast<AreaMode*>(ModeManager::GetInstance()->GetMode())->AddPoint(10);
	}
}
