//=============================================================
//
// リザルト [mt_result.cpp]
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
#include "scripts/result/result_data.h"
#include "scripts/result/result_view.h"

float MountainResultManager::m_beforeFuel = CVehicle::MAX_FUEL;
float MountainResultManager::m_beforeEndurance = CVehicle::MAX_ENDURANCE;
UINT MountainResultManager::m_goalCount = 0;
std::vector<MountainResultManager::ResultData> MountainResultManager::m_results = {};

//=============================================================
// [MountainResultManager] 結果を保存する
//=============================================================
void MountainResultManager::AddResult(ResultData data)
{
	m_results.push_back(data);
}

//=============================================================
// [MountainResultManager] 初期化
//=============================================================
void MountainResultManager::Init()
{
	// 踏破数をインクリメント
	m_goalCount++;

	// 進捗
	m_progState = P_MTTEXT;
	m_progCounter = 120;

	// クリアテキスト
	{
		m_mtText = new GameObject("MtClearText", "UI");
		m_mtText->AddComponent<CText>();
		m_mtText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_mtText->GetComponent<CText>()->SetFontSize(130);
		m_mtText->GetComponent<CText>()->SetOutlineColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		m_mtText->GetComponent<CText>()->SetOutlineSize(2);
		m_mtText->GetComponent<CText>()->SetText(std::to_string(m_goalCount) + "つ目の山を踏破しました");
		m_mtText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_mtText->transform->SetPos(-2000.0f, 100.0f, 0.0f);
	}

	// データ表示
	{
		m_dataView = new GameObject("DataView", "UI");
		m_dataView->AddComponent<ResultDataView>();
		m_dataView->transform->Translate(60.0f, 350.0f, 0.0f);
		
		// 最新データを取得
		ResultData data = m_results[m_results.size()-1];
		m_dataView->GetComponent<ResultDataView>()->SetTime(data.time);
		m_dataView->GetComponent<ResultDataView>()->SetHighSpeed(data.highSpeed);
		m_dataView->GetComponent<ResultDataView>()->SetAction(data.action);
	}

	// バイク情報表示
	{
		// 燃料
		m_fuelView = new GameObject("FuelView", "UI");
		m_fuelView->transform->SetPos(1310.0f, 360.0f);
		m_fuelView->AddComponent<ResultViewBar>(
			"燃料",
			D3DCOLOR_RGBA(232, 44, 44, 255),
			D3DCOLOR_RGBA(255, 0, 0, 255),
			D3DCOLOR_RGBA(255, 0, 0, 255)
			);
		m_fuelView->GetComponent<ResultViewBar>()->SetValue(
			static_cast<float>(m_beforeFuel / CVehicle::MAX_FUEL),
			static_cast<float>(m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel() / CVehicle::MAX_FUEL));

		// 耐久値
		m_enduranceView = new GameObject("EnduranceView", "UI");
		m_enduranceView->transform->SetPos(1310.0f, 600.0f);
		m_enduranceView->AddComponent<ResultViewBar>(
			"耐久値",
			D3DCOLOR_RGBA(78, 69, 255, 255),
			D3DCOLOR_RGBA(75, 67, 224, 255),
			D3DCOLOR_RGBA(61, 100, 255, 255)
			);
		m_enduranceView->GetComponent<ResultViewBar>()->SetValue(
			static_cast<float>(m_beforeEndurance / CVehicle::MAX_ENDURANCE),
			static_cast<float>(m_gameScene->GetBike()->GetComponent<CVehicle>()->GetEndurance() / CVehicle::MAX_ENDURANCE));
	}

	// 地形画像
	{
		m_terrainImg = new GameObject("TerrainImg", "ResultData");
		m_terrainImg->AddComponent<CPolygon>();
		m_terrainImg->GetComponent<CPolygon>()->SetTexture("data\\terrain.bmp");
		m_terrainImg->transform->SetSize(500.0f, 500.0f);
		m_terrainImg->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 250.0f, 300.0f);
	}

	// シードテキスト
	{
		m_seedText = new GameObject("SeedText", "UI");
		m_seedText->AddComponent<CText>();
		m_seedText->GetComponent<CText>()->SetFontSize(50);
		m_seedText->GetComponent<CText>()->SetText("<color=150,150,150>Seed: " + std::to_string(m_gameScene->GetTerrain()->GetSeed()));
		m_seedText->transform->SetPos(5.0f, CRenderer::SCREEN_HEIGHT - 50.0f);
	}

	// 背景
	{
		m_bg = new GameObject("BG", "UI");
		m_bg->SetPriority(3);
		m_bg->AddComponent<CPolygon>();
		m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
		m_bg->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	}

	// 前回の情報として保存
	m_beforeFuel = m_gameScene->GetBike()->GetComponent<CVehicle>()->GetFuel();
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
	m_bg->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, currentAlpha + (0.5f - currentAlpha) * 0.02f));

	// クリアテキストのアニメーション
	if (m_progState <= MountainResultManager::P_MTTEXT)
	{
		m_mtText->transform->SetPos(m_mtText->transform->GetWPos().x +
			(CRenderer::SCREEN_WIDTH / 2 - m_mtText->transform->GetWPos().x) * 0.08f, 100.0f
		);
	}

	// バイクの情報を表示
	if (m_progState == MountainResultManager::P_FUEL)
	{
		if (m_progCounter == 80)
		{
			m_fuelView->GetComponent<ResultViewBar>()->StartAnim();
		}
	}
	if (m_progState == MountainResultManager::P_ENDURANCE)
	{
		if (m_progCounter == 80)
		{
			m_enduranceView->GetComponent<ResultViewBar>()->StartAnim();
		}
	}


	// 次の状態に移行する処理
	m_progCounter--;
	if (m_progCounter <= 0 && m_progState != P_END)
	{
		m_progState = static_cast<PROG_STATE>(m_progState + 1);	
		m_progCounter = 80;
	}
}

//=============================================================
// [MountainResultManager] リセット
//=============================================================
void MountainResultManager::Reset()
{
	m_beforeFuel = CVehicle::MAX_FUEL;
	m_beforeEndurance = CVehicle::MAX_ENDURANCE;
	m_goalCount = 0;
	m_results.clear();
}