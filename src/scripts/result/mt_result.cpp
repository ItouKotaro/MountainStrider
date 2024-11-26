//=============================================================
//
// リザルト [result.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "mt_result.h"
#include "scene/game.h"

#include "renderer.h"
#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "component/2d/bar.h"

#include "scripts/vehicle.h"

float MountainResultManager::m_oldFuel = CVehicle::MAX_FUEL;
float MountainResultManager::m_oldEndurance = CVehicle::MAX_ENDURANCE;
UINT MountainResultManager::m_goalCount = 0;

//=============================================================
// [MountainResultManager] 初期化
//=============================================================
void MountainResultManager::Init()
{
	// 踏破数をインクリメント
	m_goalCount++;

	// 進捗
	m_progState = P_MT_TEXT;
	m_progCounter = 120;

	// スコアテキスト
	m_mtText = new GameObject("MtClearText", "UI");
	m_mtText->AddComponent<CText>();
	m_mtText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_mtText->GetComponent<CText>()->SetFontSize(130);
	m_mtText->GetComponent<CText>()->SetOutlineColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_mtText->GetComponent<CText>()->SetOutlineSize(5);
	m_mtText->GetComponent<CText>()->SetText(std::to_string(m_goalCount) + "つ目の山を踏破しました");
	m_mtText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_mtText->transform->SetPos(-2000.0f, 120.0f, 0.0f);

	// シードテキスト
	m_seedText = new GameObject("SeedText", "UI");
	m_seedText->AddComponent<CText>();
	m_seedText->GetComponent<CText>()->SetFontSize(50.0f);
	m_seedText->GetComponent<CText>()->SetText("<color=150,150,150>Seed: " + std::to_string(m_gameScene->GetTerrain()->GetSeed()));
	m_seedText->transform->SetPos(5.0f, CRenderer::SCREEN_HEIGHT - 50.0f);

	// 燃料バー
	m_fuelBar = new GameObject("FuelBar", "UI");
	m_fuelBar->transform->SetPos(CRenderer::SCREEN_WIDTH/2, 500.0f);
	m_fuelBar->AddComponent<CAdvancedBar>();
	m_fuelBar->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
	m_fuelBar->GetComponent<CAdvancedBar>()->SetLength(800.0f);
	m_fuelBar->GetComponent<CAdvancedBar>()->SetBold(50.0f);
	m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(0, D3DCOLOR_RGBA(252, 185, 40, 255));
	m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(2, D3DCOLOR_RGBA(252, 185, 40, 255));
	m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(1, D3DCOLOR_RGBA(255, 0, 0, 255));
	m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(3, D3DCOLOR_RGBA(255, 0, 0, 255));


	// 耐久値バー
	m_enduranceBar = new GameObject("FuelBar", "UI");
	m_enduranceBar->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 700.0f);
	m_enduranceBar->AddComponent<CAdvancedBar>();
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetLength(800.0f);
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetBold(50.0f);
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(0, D3DCOLOR_RGBA(7, 232, 104, 255));
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(2, D3DCOLOR_RGBA(7, 232, 104, 255));
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(1, D3DCOLOR_RGBA(10, 201, 163, 255));
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(3, D3DCOLOR_RGBA(10, 201, 163, 255));


	// 背景
	m_bg = new GameObject("BG", "UI");
	m_bg->SetPriority(3);
	m_bg->AddComponent<CPolygon>();
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
	m_bg->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
}

//=============================================================
// [MountainResultManager] 終了
//=============================================================
void MountainResultManager::Uninit()
{
	m_mtText->Destroy();
	m_bg->Destroy();

}

//=============================================================
// [MountainResultManager] 更新
//=============================================================
void MountainResultManager::Update()
{
	// 背景のフェード
	float currentAlpha = m_bg->GetComponent<CPolygon>()->GetColor().a;
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, currentAlpha + (0.2f - currentAlpha) * 0.02f));

	switch (m_progState)
	{
	case MountainResultManager::P_MT_TEXT:
		m_mtText->transform->SetPos(m_mtText->transform->GetWPos().x +
			(CRenderer::SCREEN_WIDTH / 2 - m_mtText->transform->GetWPos().x) * 0.08f, 120.0f
		);
		break;
	case MountainResultManager::P_FUEL_TEXT:
		break;
	case MountainResultManager::P_FUEL_BAR:
		break;
	case MountainResultManager::P_ENDURANCE_TEXT:
		break;
	case MountainResultManager::P_ENDURANCE_BAR:
		break;
	case MountainResultManager::P_END:
		break;
	default:
		break;
	}

	m_progCounter--;
	if (m_progCounter <= 0 && m_progState != P_END)
	{
		m_progState = static_cast<PROG_STATE>(m_progState + 1);	
		m_progCounter = 60;
	}
}