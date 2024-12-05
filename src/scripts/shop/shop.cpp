//=============================================================
//
// ショップ [shop.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "shop.h"

#include "renderer.h"
#include "scripts/result/mt_result.h"
#include "scene/game.h"
#include "scripts/vehicle.h"

#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "scripts/result/result_view.h"
#include "component/other/button.h"

const float ShopManager::BAR_SPACE = 110.0f;

//=============================================================
// [ShopManager] 初期化
//=============================================================
void ShopManager::Init()
{
	// ゲームシーンの取得
	m_gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	// ショップの作成
	m_shop = new GameObject();
	m_shop->SetVisible(false);

	// ポイント表示
	m_ptView = new GameObject();
	m_ptView->SetParent(m_shop);
	m_ptView->SetPriority(6);
	m_ptView->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	m_ptView->AddComponent<CPolygon>();
	m_ptView->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SHOP\\shop.png");

	// 燃料
	{
		m_fuel = new GameObject();
		m_fuel->SetParent(m_shop);
		m_fuel->transform->SetPos(static_cast<float>(-CRenderer::SCREEN_WIDTH / 2), 0.0f);

		// 背景
		GameObject* fuelBG = new GameObject();
		fuelBG->SetParent(m_fuel);
		fuelBG->SetPriority(5);
		fuelBG->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(201, 24, 0, 255));
		fuelBG->GetComponent<CPolygon>()->SetColor(1, D3DCOLOR_RGBA(250, 176, 97, 255));
		fuelBG->GetComponent<CPolygon>()->SetColor(3, D3DCOLOR_RGBA(250, 176, 97, 255));
		fuelBG->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 280.0f);

		// テキスト
		GameObject* fuelText = new GameObject();
		fuelText->transform->SetPos(120.0f, 40.0f);
		fuelText->SetParent(m_fuel);
		fuelText->SetPriority(6);
		fuelText->AddComponent<CText>()->SetText("燃料");
		fuelText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		fuelText->GetComponent<CText>()->SetFontSize(80);

		// バー
		m_fuelBar = new GameObject();
		m_fuelBar->SetParent(m_fuel);
		m_fuelBar->SetPriority(7);
		m_fuelBar->transform->SetPos(CRenderer::SCREEN_WIDTH/4 - BAR_SPACE, 150.0f);
		m_fuelBar->AddComponent<CAdvancedBar>();
		m_fuelBar->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
		m_fuelBar->GetComponent<CAdvancedBar>()->SetLength(500.0f);
		m_fuelBar->GetComponent<CAdvancedBar>()->SetBold(30.0f);
		m_fuelBar->GetComponent<CAdvancedBar>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		m_fuelBar->GetComponent<CAdvancedBar>()->SetTexture("data\\TEXTURE\\RESULT\\hand_bar.png");
		m_fuelBar->GetComponent<CAdvancedBar>()->SetProgress(pBike->GetFuel() / static_cast<float>(CVehicle::MAX_FUEL));

		// 現在値と最大値
		m_fuelCurrentValue = new GameObject();
		m_fuelCurrentValue->SetParent(m_fuel);
		m_fuelCurrentValue->SetPriority(7);
		m_fuelCurrentValue->transform->SetPos(620.0f, 90.0f);
		m_fuelCurrentValue->AddComponent<CText>()->SetText(std::to_string(static_cast<int>(pBike->GetFuel()))+"<size=20>/"+std::to_string(static_cast<int>(CVehicle::MAX_FUEL)));
		m_fuelCurrentValue->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_fuelCurrentValue->GetComponent<CText>()->SetFontSize(40);
		m_fuelCurrentValue->GetComponent<CText>()->SetAlign(CText::RIGHT);

		// 購入ボタン
		m_fuelButton = new GameObject();
		m_fuelButton->SetParent(m_fuel);
		m_fuelButton->transform->SetSize(300.0f, 90.0f);
		m_fuelButton->transform->SetPos(230.0f, 180.0f);
		m_fuelButton->AddComponent<ButtonUI>();
		m_fuelButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\SHOP\\maintenance_buy.png");

		// コストアイコン
		GameObject* fuelCostIcon = new GameObject();
		fuelCostIcon->SetParent(m_fuel);
		fuelCostIcon->transform->SetPos(235.0f, 188.0f);
		fuelCostIcon->transform->SetSize(40.0f, 40.0f);
		fuelCostIcon->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SHOP\\point.png");

		// コスト表示
		m_fuelCost = new GameObject();
		m_fuelCost->SetParent(fuelCostIcon);
		m_fuelCost->transform->SetPos(60.0f, 9.0f);
		m_fuelCost->AddComponent<CText>()->SetText("5");
		m_fuelCost->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_fuelCost->GetComponent<CText>()->SetFontSize(36);
		m_fuelCost->GetComponent<CText>()->SetAlign(CText::CENTER);

		// 増加量表示
		m_fuelButtonText = new GameObject();
		m_fuelButtonText->SetParent(m_fuelButton);
		m_fuelButtonText->transform->SetPos(195.0f, 23.0f);
		m_fuelButtonText->AddComponent<CText>()->SetText("+100");
		m_fuelButtonText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_fuelButtonText->GetComponent<CText>()->SetFontSize(60);
		m_fuelButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
	}
	
	// 耐久値
	{
		m_endurance = new GameObject();
		m_endurance->SetParent(m_shop);
		m_endurance->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH), 0.0f);

		// 背景
		GameObject* enduranceBG = new GameObject();
		enduranceBG->SetParent(m_endurance);
		enduranceBG->SetPriority(5);
		enduranceBG->AddComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(15, 92, 15, 255));
		enduranceBG->GetComponent<CPolygon>()->SetColor(1, D3DCOLOR_RGBA(24, 184, 90, 255));
		enduranceBG->GetComponent<CPolygon>()->SetColor(3, D3DCOLOR_RGBA(24, 184, 90, 255));
		enduranceBG->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 280.0f);

		// テキスト
		GameObject* enduranceText = new GameObject();
		enduranceText->transform->SetPos(340.0f, 40.0f);
		enduranceText->SetParent(m_endurance);
		enduranceText->SetPriority(6);
		enduranceText->AddComponent<CText>()->SetText("耐久値");
		enduranceText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		enduranceText->GetComponent<CText>()->SetFontSize(80);

		// バー
		m_enduranceBar = new GameObject();
		m_enduranceBar->SetParent(m_endurance);
		m_enduranceBar->SetPriority(7);
		m_enduranceBar->transform->SetPos(CRenderer::SCREEN_WIDTH / 4 + BAR_SPACE, 150.0f);
		m_enduranceBar->AddComponent<CAdvancedBar>();
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetLength(500.0f);
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetBold(30.0f);
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetTexture("data\\TEXTURE\\RESULT\\hand_bar.png");
		m_enduranceBar->GetComponent<CAdvancedBar>()->SetProgress(pBike->GetEndurance() / static_cast<float>(CVehicle::MAX_ENDURANCE));

		// 現在値と最大値
		m_enduranceCurrentValue = new GameObject();
		m_enduranceCurrentValue->SetParent(m_endurance);
		m_enduranceCurrentValue->SetPriority(7);
		m_enduranceCurrentValue->transform->SetPos(837.5f, 90.0f);
		m_enduranceCurrentValue->AddComponent<CText>()->SetText(std::to_string(static_cast<int>(pBike->GetEndurance())) + "<size=20>/" + std::to_string(static_cast<int>(CVehicle::MAX_ENDURANCE)));
		m_enduranceCurrentValue->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_enduranceCurrentValue->GetComponent<CText>()->SetFontSize(40);
		m_enduranceCurrentValue->GetComponent<CText>()->SetAlign(CText::RIGHT);

		// 購入ボタン
		m_enduranceButton = new GameObject();
		m_enduranceButton->SetParent(m_endurance);
		m_enduranceButton->transform->SetSize(300.0f, 90.0f);
		m_enduranceButton->transform->SetPos(440.0f, 180.0f);
		m_enduranceButton->AddComponent<ButtonUI>();
		m_enduranceButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\SHOP\\maintenance_buy.png");

		// コストアイコン
		GameObject* enduranceCostIcon = new GameObject();
		enduranceCostIcon->SetParent(m_endurance);
		enduranceCostIcon->transform->SetPos(445.0f, 188.0f);
		enduranceCostIcon->transform->SetSize(40.0f, 40.0f);
		enduranceCostIcon->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SHOP\\point.png");

		// コスト表示
		m_enduranceCost = new GameObject();
		m_enduranceCost->SetParent(enduranceCostIcon);
		m_enduranceCost->transform->SetPos(60.0f, 9.0f);
		m_enduranceCost->AddComponent<CText>()->SetText("5");
		m_enduranceCost->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_enduranceCost->GetComponent<CText>()->SetFontSize(36);
		m_enduranceCost->GetComponent<CText>()->SetAlign(CText::CENTER);

		// 増加量表示
		m_enduranceButtonText = new GameObject();
		m_enduranceButtonText->SetParent(m_enduranceButton);
		m_enduranceButtonText->transform->SetPos(195.0f, 23.0f);
		m_enduranceButtonText->AddComponent<CText>()->SetText("+100");
		m_enduranceButtonText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_enduranceButtonText->GetComponent<CText>()->SetFontSize(60);
		m_enduranceButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
	}
}

//=============================================================
// [ShopManager] 終了
//=============================================================
void ShopManager::Uninit()
{
}

//=============================================================
// [ShopManager] 更新
//=============================================================
void ShopManager::Update()
{
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	// 燃料と耐久値のアニメーション
	m_fuel->transform->Translate((D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_fuel->transform->GetWPos()) * 0.08f);
	m_endurance->transform->Translate((D3DXVECTOR3(static_cast<float>(CRenderer::SCREEN_WIDTH/2), 0.0f, 0.0f) - m_endurance->transform->GetWPos()) * 0.08f);

	// バーの更新
	m_fuelBar->GetComponent<CAdvancedBar>()->SetProgress(pBike->GetFuel() / static_cast<float>(CVehicle::MAX_FUEL));
	m_fuelBar->GetComponent<CAdvancedBar>()->UpdateVertex();
	m_fuelCurrentValue->GetComponent<CText>()->SetText(std::to_string(static_cast<int>(pBike->GetFuel())) + "<size=20>/" + std::to_string(static_cast<int>(CVehicle::MAX_FUEL)));
	
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetProgress(pBike->GetEndurance() / static_cast<float>(CVehicle::MAX_ENDURANCE));
	m_enduranceBar->GetComponent<CAdvancedBar>()->UpdateVertex();
	m_enduranceCurrentValue->GetComponent<CText>()->SetText(std::to_string(static_cast<int>(pBike->GetEndurance())) + "<size=20>/" + std::to_string(static_cast<int>(CVehicle::MAX_ENDURANCE)));

}

//=============================================================
// [ShopManager] 描画
//=============================================================
void ShopManager::Draw()
{
}
