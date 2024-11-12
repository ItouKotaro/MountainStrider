//=============================================================
//
// ゲームマネージャー [game_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "game_manager.h"

#include "scripts/status_ui.h"

// 静的メンバ変数の初期化
const int CGameManager::MAX_FUEL = 1500;
const int CGameManager::MAX_ENDURANCE = 200;

//=============================================================
// [CGameManager] 初期化
//=============================================================
void CGameManager::Init()
{
	// 変数の初期化
	m_nFuel = MAX_FUEL;
	m_nEndurance = MAX_ENDURANCE;

	m_pStatusUI = new GameObject("StatusUI", "UI");
	m_pStatusUI->AddComponent<CStatusUI>();
}

//=============================================================
// [CGameManager] 終了
//=============================================================
void CGameManager::Uninit()
{
	m_pStatusUI->Destroy();
}

//=============================================================
// [CGameManager] 更新
//=============================================================
void CGameManager::Update()
{
	m_pStatusUI->GetComponent<CStatusUI>()->SetFuel(m_nFuel / static_cast<float>(MAX_FUEL));
	m_pStatusUI->GetComponent<CStatusUI>()->SetEndurance(m_nEndurance / static_cast<float>(MAX_ENDURANCE));
}