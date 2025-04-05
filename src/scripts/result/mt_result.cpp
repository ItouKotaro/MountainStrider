//=============================================================
//
// リザルト [mt_result.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "mt_result.h"
#include "scene/game.h"
#include "manager.h"

#include "renderer.h"
#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "component/2d/bar.h"
#include "component/other/page.h"

#include "scripts/vehicle.h"
#include "scripts/result/result_data.h"
#include "scripts/result/result_view.h"
#include "scripts/result/result_terrain.h"
#include "component/other/button.h"
#include "scripts/virtual_cursor.h"
#include "scripts/ranking_system.h"

#include "scripts/mode/terminal.h"

float TerminalResult::m_beforeFuel = CVehicle::MAX_FUEL;
float TerminalResult::m_beforeEndurance = CVehicle::MAX_ENDURANCE;

//=============================================================
// [ResultBase] コンストラクタ
//=============================================================
ResultBase::ResultBase()
{
	// ゲームシーンを取得する
	m_gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
}