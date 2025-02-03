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

//=============================================================
// [TerminalMode] 初期化
//=============================================================
void TerminalMode::Init()
{

}

//=============================================================
// [TerminalMode] 終了
//=============================================================
void TerminalMode::Uninit()
{

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
}