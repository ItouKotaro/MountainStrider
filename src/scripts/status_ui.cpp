//=============================================================
//
// ステータスUI [status_ui.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "status_ui.h"

//=============================================================
// [CStatusUI] 初期化
//=============================================================
void CStatusUI::Init()
{
	GameObject* pFuelObj = new GameObject;
	GameObject* pEnduranceObj = new GameObject;

	// オブジェクトを生成する
	m_pFuelBar = pFuelObj->AddComponent<CAdvancedBar>();
	m_pEnduranceBar = pEnduranceObj->AddComponent<CAdvancedBar>();

	// バーの位置を変更する
	pFuelObj->transform->SetPos(50.0f, 100.0f);
	pEnduranceObj->transform->SetPos(50.0f, 150.0f);

	// バーの設定
	m_pFuelBar->SetColor(0, D3DCOLOR_RGBA(252, 185, 40, 255));
	m_pFuelBar->SetColor(2, D3DCOLOR_RGBA(252, 185, 40, 255));
	m_pFuelBar->SetColor(1, D3DCOLOR_RGBA(255, 0, 0, 255));
	m_pFuelBar->SetColor(3, D3DCOLOR_RGBA(255, 0, 0, 255));

	m_pEnduranceBar->SetColor(0, D3DCOLOR_RGBA(7, 232, 104, 255));
	m_pEnduranceBar->SetColor(2, D3DCOLOR_RGBA(7, 232, 104, 255));
	m_pEnduranceBar->SetColor(1, D3DCOLOR_RGBA(10, 201, 163, 255));
	m_pEnduranceBar->SetColor(3, D3DCOLOR_RGBA(10, 201, 163, 255));
}

//=============================================================
// [CStatusUI] 終了
//=============================================================
void CStatusUI::Uninit()
{
	m_pFuelBar->gameObject->Destroy();
	m_pEnduranceBar->gameObject->Destroy();
}

//=============================================================
// [CStatusUI] 燃料の設定
//=============================================================
void CStatusUI::SetFuel(const float& percent)
{
	if (percent != m_pFuelBar->GetProgress())
	{
		m_pFuelBar->SetProgress(percent);
	}
}

//=============================================================
// [CStatusUI] 耐久値の設定
//=============================================================
void CStatusUI::SetEndurance(const float& percent)
{
	if (percent != m_pEnduranceBar->GetProgress())
	{
		m_pEnduranceBar->SetProgress(percent);
	}
}