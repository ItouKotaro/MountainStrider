//=============================================================
//
// モード管理 [mode_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "mode_manager.h"
#include "manager.h"

//=============================================================
// [ModeManager] 初期化
//=============================================================
void ModeManager::Init()
{
	if (m_mode != nullptr)
	{
		m_mode->Init();
	}
}

//=============================================================
// [ModeManager] 終了
//=============================================================
void ModeManager::Uninit()
{
	if (m_mode != nullptr)
	{
		m_mode->Uninit();
	}

	if (m_result != nullptr)
	{
		m_result->Uninit();
		delete m_result;
		m_result = nullptr;
	}
}

//=============================================================
// [ModeManager] 更新
//=============================================================
void ModeManager::Update()
{
	if (m_mode != nullptr)
	{
		m_mode->Update();
	}
}

//=============================================================
// [ModeManager] 最後の終了
//=============================================================
void ModeManager::LastUninit()
{
	if (m_mode != nullptr)
	{
		delete m_mode;
		m_mode = nullptr;
	}
}

//=============================================================
// [ModeManager] モード設定
//=============================================================
void ModeManager::SetMode(ModeTemplate* mode)
{
	// 既にモードが設定されている場合は終了する
	if (m_mode != nullptr)
	{
		m_mode->Uninit();
		delete m_mode;
		m_mode = nullptr;
	}

	// 新しいモードを設定する
	m_mode = mode;
}

//=============================================================
// [ModeManager] リザルトを設定する
//=============================================================
void ModeManager::SetResult(ResultBase* result)
{
	// リザルトがあるときは削除する
	if (m_result != nullptr)
	{
		m_result->Uninit();
		delete m_result;
		m_result = nullptr;
	}

	// リザルトデータを計算する
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	gameScene->CalcResultData();

	// リザルトを設定する
	m_result = result;
	m_result->Init();
}

//=============================================================
// [ModeTemplate] コンストラクタ
//=============================================================
ModeTemplate::ModeTemplate()
{
	// ゲームシーンを取得する
	m_game = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
}
