//=============================================================
//
// 終端モード [terminal.cpp]
// Author: Ito Kotaro
// 
// ワールドの端まで到達するとゴールする
//
//=============================================================
#include "terminal.h"
#include "scene/game.h"

#include "scripts/direction_arrow.h"

//=============================================================
// [TerminalMode] 初期化
//=============================================================
void TerminalMode::Init()
{
	// 方向矢印を生成する
	m_directionObj = new GameObject("DirectionArrow");
	m_directionObj->transform->SetPos(0.0f, 0.0f, 50.0f);
	m_directionObj->AddComponent<DirectionArrow>();
}

//=============================================================
// [TerminalMode] 終了
//=============================================================
void TerminalMode::Uninit()
{
	m_directionObj->Destroy();
}

//=============================================================
// [TerminalMode] 更新
//=============================================================
void TerminalMode::Update()
{
	// バイクの位置を取得する
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	D3DXVECTOR3 vehiclePos = gameScene->GetBike()->transform->GetWPos();

	// 地形の端に行ったとき
	if (vehiclePos.x <= -Terrain::TERRAIN_DISTANCE_HALF + EXTENSION_DISTANCE || vehiclePos.x >= Terrain::TERRAIN_DISTANCE_HALF - EXTENSION_DISTANCE ||
		vehiclePos.z <= -Terrain::TERRAIN_DISTANCE_HALF + EXTENSION_DISTANCE || vehiclePos.z >= Terrain::TERRAIN_DISTANCE_HALF - EXTENSION_DISTANCE)
	{
		onGoal();
	}

	// 最低高度よりも下に行ったとき
	if (vehiclePos.y < gameScene->GetTerrain()->GetMinHeight() - 5.0f)
	{
		onGoal();
	}

	// 方向矢印の目的位置を更新する
	m_directionObj->GetComponent<DirectionArrow>()->SetDestination(CalcNearGoal());
}

//=============================================================
// [TerminalMode] 最も近いゴールの位置を取得する
//=============================================================
D3DXVECTOR3 TerminalMode::CalcNearGoal()
{
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	D3DXVECTOR3 vehiclePos = gameScene->GetBike()->transform->GetWPos();

	// 上下左右のどの地点が一番近いか
	enum NEAR_PT
	{
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};
	NEAR_PT nearPt;

	// 一番近いポイントを導き出す
	float nearDis = Terrain::TERRAIN_DISTANCE;
	if (fabsf(vehiclePos.x - Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = LEFT;
		nearDis = fabsf(vehiclePos.x - Terrain::TERRAIN_DISTANCE_HALF);
	}
	if (fabsf(vehiclePos.x + Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = RIGHT;
		nearDis = fabsf(vehiclePos.x + Terrain::TERRAIN_DISTANCE_HALF);
	}
	if (fabsf(vehiclePos.z + Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = TOP;
		nearDis = fabsf(vehiclePos.z + Terrain::TERRAIN_DISTANCE_HALF);
	}
	if (fabsf(vehiclePos.z - Terrain::TERRAIN_DISTANCE_HALF) < nearDis)
	{
		nearPt = BOTTOM;
		nearDis = fabsf(vehiclePos.z - Terrain::TERRAIN_DISTANCE_HALF);
	}

	// それぞれの方向の目標地点を設定する
	D3DXVECTOR3 destination = { 0.0f, 0.0f, 0.0f };
	switch (nearPt)
	{
	case TOP:
		destination = { 0.0f, 0.0f, -Terrain::TERRAIN_DISTANCE_HALF };
		break;
	case BOTTOM:
		destination = { 0.0f, 0.0f, Terrain::TERRAIN_DISTANCE_HALF };
		break;
	case LEFT:
		destination = { Terrain::TERRAIN_DISTANCE_HALF, 0.0f, 0.0f };
		break;
	case RIGHT:
		destination = { -Terrain::TERRAIN_DISTANCE_HALF, 0.0f, 0.0f };
		break;
	}

	return destination;
}