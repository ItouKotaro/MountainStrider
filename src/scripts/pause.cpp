//=============================================================
//
// ポーズ [pause.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "pause.h"
#include "renderer.h"
#include "component/other/page.h"
#include "component/other/button.h"
#include "scripts/camera_move.h"
#include "internal/physics.h"

//=============================================================
// [Pause] 初期化
//=============================================================
void Pause::Init()
{
	m_pausePage = new GameObject();
	auto page = m_pausePage->AddComponent<Pages>();
	page->SetNumPage(2);

	// 戻るボタン
	m_backButton = new GameObject();
	m_backButton->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 250.0f, 300.0f);
	m_backButton->transform->SetSize(500.0f, 100.0f);
	m_backButton->AddComponent<ButtonUI>();
	page->AddObject(1, m_backButton);

	// 終了ボタン
	m_endButton = new GameObject();
	m_endButton->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 250.0f, 600.0f);
	m_endButton->transform->SetSize(500.0f, 100.0f);
	m_endButton->AddComponent<ButtonUI>();
	page->AddObject(1, m_endButton);

	// ページを変更する
	page->AllHideObjects();
}

//=============================================================
// [Pause] ポーズにするか
//=============================================================
void Pause::SetPause(const bool& enabled)
{
	m_pausePage->GetComponent<Pages>()->SetPage(enabled ? 1 : 0);
	m_isPause = enabled;

	// 物理設定
	CPhysics::GetInstance()->SetUpdatePhysics(!m_isPause);
	GameObject::Find("Camera")->GetComponent<CCameraMove>()->enabled = !enabled;
}
