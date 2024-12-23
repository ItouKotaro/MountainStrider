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
	m_fuelIcon = new SingleComponent<CPolygon>();
	m_enduranceIcon = new SingleComponent<CPolygon>();
	m_fuelIcon->Init();
	m_enduranceIcon->Init();
	
	// アイコンの設定
	m_fuelIcon->SetParent(pFuelObj);
	m_enduranceIcon->SetParent(pEnduranceObj);
	m_fuelIcon->transform->SetPos(-60.0f, -25.0f);
	m_enduranceIcon->transform->SetPos(-60.0f, -25.0f);
	m_fuelIcon->transform->SetSize(45.0f, 45.0f);
	m_enduranceIcon->transform->SetSize(45.0f, 45.0f);
	m_fuelIcon->SetTexture("data\\TEXTURE\\UI\\fuel_icon.png");
	m_enduranceIcon->SetTexture("data\\TEXTURE\\UI\\endurance_icon.png");

	// オブジェクトを生成する
	m_pFuelBar = pFuelObj->AddComponent<CAdvancedBar>();
	m_pEnduranceBar = pEnduranceObj->AddComponent<CAdvancedBar>();

	// バーの位置を変更する
	pFuelObj->transform->SetPos(100.0f, 60.0f);
	pEnduranceObj->transform->SetPos(100.0f, 120.0f);

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
	m_fuelIcon->Uninit();
	delete m_fuelIcon;
	m_enduranceIcon->Uninit();
	delete m_enduranceIcon;
}

//=============================================================
// [CStatusUI] 更新
//=============================================================
void CStatusUI::Update()
{
	m_fuelIcon->Update();
	m_enduranceIcon->Update();
}

//=============================================================
// [CStatusUI] UI描画
//=============================================================
void CStatusUI::DrawUI()
{
	m_fuelIcon->DrawUI();
	m_enduranceIcon->DrawUI();
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

//=============================================================
// [CStatusUI] 表示設定
//=============================================================
void CStatusUI::SetVisible(const bool& visible)
{
	m_pFuelBar->gameObject->SetVisible(visible);
	m_pEnduranceBar->gameObject->SetVisible(visible);
	m_fuelIcon->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, visible ? 1.0f : 0.0f));
	m_enduranceIcon->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, visible ? 1.0f : 0.0f));
}