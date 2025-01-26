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
#include "scripts/shop/buy_button.h"
#include "scripts/shop/inventory.h"

const float ShopManager::BAR_SPACE = 110.0f;
int ShopManager::m_points = 100;

//=============================================================
// [ShopManager] 初期化
//=============================================================
void ShopManager::Init(Pages* pages)
{
	m_pages = pages;
	m_viewPoints = m_points;
	m_pointsCounter = 0;

	// 音を生成する
	m_buySE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\SHOP\\buy.mp3", FMOD_2D);

	// アイテムのリスト追加処理
	RegisterItemList();

	// ゲームシーンの取得
	m_gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	// ポイント表示
	m_ptView = new GameObject();
	m_ptView->SetPriority(6);
	m_ptView->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	m_ptView->AddComponent<CPolygon>();
	m_ptView->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SHOP\\shop.png");
	m_pages->AddObject(1, m_ptView);

	// ポイントの数字テキスト
	m_ptText = new GameObject();
	m_ptText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 + 50.0f, 100.0f);
	m_ptText->SetPriority(7);
	m_ptText->AddComponent<CText>()->SetText("<color=23,116,255>"+std::to_string(m_points));
	m_ptText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_ptText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_ptText->GetComponent<CText>()->SetFontSize(120);
	m_pages->AddObject(1, m_ptText);

	// ショップアイテムボタンを作成する
	for (int i = 0; i < 6; i++)
	{
		m_shopItems[i] = new GameObject();
		m_shopItems[i]->transform->SetPos(300.0f * i + 40.0f, 350.0f);
		m_shopItems[i]->transform->Translate(i < 3 ? 0.0f : 60.0f, 0.0f, 0.0f);
		m_pages->AddObject(1, m_shopItems[i]);
	}

	// アイテムを設定する
	m_shopItems[0]->AddComponent<BuyButtonUI>(m_itemList[0]);
	m_shopItems[1]->AddComponent<BuyButtonUI>(m_itemList[1]);
	m_shopItems[2]->AddComponent<BuyButtonUI>(m_itemList[2]);
	m_shopItems[3]->AddComponent<BuyButtonUI>(m_itemList[3]);
	m_shopItems[4]->AddComponent<BuyButtonUI>(m_itemList[4]);
	m_shopItems[5]->AddComponent<BuyButtonUI>(m_itemList[2]);

	// インベントリを作成する
	m_inventory = new GameObject("Inventory", "UI");
	m_inventory->AddComponent<InventoryUI>();
	m_inventory->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2) - 450.0f, 820.0f);
	m_pages->AddObject(1, m_inventory);

	// インベントリ説明
	GameObject* inventoryInfo = new GameObject;
	inventoryInfo->AddComponent<CText>()->SetText("インベントリ内のアイテムをクリックで持ち込む");
	inventoryInfo->GetComponent<CText>()->SetAlign(CText::CENTER);
	inventoryInfo->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	inventoryInfo->GetComponent<CText>()->SetFontSize(40);
	inventoryInfo->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), static_cast<float>(CRenderer::SCREEN_HEIGHT - 60.0f));
	m_pages->AddObject(1, inventoryInfo);

	// 次へのボタンを作成する
	GameObject* nextButton = new GameObject("NextButton", "UI");
	nextButton->transform->SetSize(400.0f, 130.0f);
	nextButton->transform->SetPos(1450.0f, 900.0f);
	nextButton->AddComponent<ButtonUI>();
	nextButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
	nextButton->GetComponent<ButtonUI>()->setClickEvent([]() {
		// 読み込み中の画像を表示
		GameObject* loadingPoly = new GameObject();
		loadingPoly->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\TITLE\\loading.png");
		loadingPoly->transform->SetSize(1220.0f, 304.0f);
		loadingPoly->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 - 610.0f, CRenderer::SCREEN_HEIGHT / 2 - 152.0f);

		CSceneManager::GetInstance()->ReloadScene(); 
		});
	m_pages->AddObject(1, nextButton);

	GameObject* nextButtonText = new GameObject("NextButtonText", "UI");
	nextButtonText->AddComponent<CText>();
	nextButtonText->GetComponent<CText>()->SetText("次の山へ");
	nextButtonText->GetComponent<CText>()->SetFontColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	nextButtonText->GetComponent<CText>()->SetFontSize(90);
	nextButtonText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	nextButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
	nextButtonText->transform->SetPos(200.0f, 25.0f);
	nextButtonText->SetParent(nextButton);
	m_pages->AddObject(1, nextButtonText);

	// 燃料と耐久値の購入ディスプレイを初期化する
	InitTopDisplay();

	// カーソル非表示
	Main::SetShowCursor(false);
}

//=============================================================
// [ShopManager] ディスプレイの初期化
//=============================================================
void ShopManager::InitTopDisplay()
{
	// ゲームシーンの取得
	m_gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	for (int idx = 0; idx < 2; idx++)
	{
		// 統括オブジェクト
		GameObject* uniteObj = new GameObject();
		m_pages->AddObject(1, uniteObj);
		uniteObj->transform->SetPos(static_cast<float>(idx == 0 ? -CRenderer::SCREEN_WIDTH / 2 : CRenderer::SCREEN_WIDTH), 0.0f);
		if (idx == 0) m_fuel = uniteObj;
		else m_endurance = uniteObj;

		// 背景
		GameObject* bgObj = new GameObject();
		bgObj->SetParent(uniteObj);
		bgObj->SetPriority(5);
		bgObj->AddComponent<CPolygon>()->SetColor(idx == 0 ? D3DCOLOR_RGBA(201, 24, 0, 255) : D3DCOLOR_RGBA(15, 92, 15, 255));
		bgObj->GetComponent<CPolygon>()->SetColor(1, idx == 0 ? D3DCOLOR_RGBA(250, 176, 97, 255) : D3DCOLOR_RGBA(24, 184, 90, 255));
		bgObj->GetComponent<CPolygon>()->SetColor(3, idx == 0 ? D3DCOLOR_RGBA(250, 176, 97, 255) : D3DCOLOR_RGBA(24, 184, 90, 255));
		bgObj->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 280.0f);
		m_pages->AddObject(1, bgObj);

		// テキスト
		GameObject* textObj = new GameObject();
		textObj->transform->SetPos(idx == 0 ? 120.0f : 340.0f, 40.0f);
		textObj->SetParent(uniteObj);
		textObj->SetPriority(6);
		textObj->AddComponent<CText>()->SetText(idx == 0 ? "燃料" : "耐久値");
		textObj->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		textObj->GetComponent<CText>()->SetFontSize(80);
		m_pages->AddObject(1, textObj);

		// バー
		GameObject* barObj = new GameObject();
		barObj = new GameObject();
		barObj->SetParent(uniteObj);
		barObj->SetPriority(7);
		m_pages->AddObject(1, barObj);
		barObj->transform->SetPos(idx == 0 ? CRenderer::SCREEN_WIDTH / 4 - BAR_SPACE : CRenderer::SCREEN_WIDTH / 4 + BAR_SPACE, 150.0f);
		barObj->AddComponent<CAdvancedBar>();
		barObj->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
		barObj->GetComponent<CAdvancedBar>()->SetLength(500.0f);
		barObj->GetComponent<CAdvancedBar>()->SetBold(30.0f);
		barObj->GetComponent<CAdvancedBar>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		barObj->GetComponent<CAdvancedBar>()->SetTexture("data\\TEXTURE\\RESULT\\hand_bar.png");
		barObj->GetComponent<CAdvancedBar>()->SetProgress(idx == 0 ? pBike->GetFuel() / static_cast<float>(CVehicle::MAX_FUEL) : pBike->GetEndurance() / static_cast<float>(CVehicle::MAX_ENDURANCE));
		if (idx == 0) m_fuelBar = barObj;
		else m_enduranceBar = barObj;

		// 現在値と最大値
		GameObject* currentValueObj = new GameObject();
		currentValueObj->SetParent(uniteObj);
		currentValueObj->SetPriority(7);
		m_pages->AddObject(1, currentValueObj);
		currentValueObj->transform->SetPos(idx == 0 ? 620.0f : 837.5f, 90.0f);
		currentValueObj->AddComponent<CText>()->SetText(std::to_string(static_cast<int>(idx == 0 ? pBike->GetFuel() : pBike->GetEndurance())) + "<size=20>/" + std::to_string(static_cast<int>(idx == 0 ? CVehicle::MAX_FUEL : CVehicle::MAX_ENDURANCE)));
		currentValueObj->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		currentValueObj->GetComponent<CText>()->SetFontSize(40);
		currentValueObj->GetComponent<CText>()->SetAlign(CText::RIGHT);
		if (idx == 0) m_fuelCurrentValue = currentValueObj;
		else m_enduranceCurrentValue = currentValueObj;

		// 購入ボタン
		GameObject* buttonObj = new GameObject();
		buttonObj->SetParent(uniteObj);
		m_pages->AddObject(1, buttonObj);
		buttonObj->transform->SetSize(300.0f, 90.0f);
		buttonObj->transform->SetPos(idx == 0 ? D3DXVECTOR2(230.0f, 180.0f) : D3DXVECTOR2(440.0f, 180.0f));
		buttonObj->AddComponent<ButtonUI>();
		buttonObj->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\SHOP\\maintenance_buy.png");
		buttonObj->GetComponent<ButtonUI>()->setClickEvent([this, buttonObj, idx]() {
			if (idx ==0) m_fuelItem.onBuy();
			else m_enduranceItem.onBuy();
			});
		if (idx == 0) m_fuelButton = buttonObj;
		else m_enduranceButton = buttonObj;

		// コストアイコン
		GameObject* costIconObj = new GameObject();
		costIconObj->SetParent(uniteObj);
		costIconObj->transform->SetPos(idx == 0 ? D3DXVECTOR2(235.0f, 188.0f) : D3DXVECTOR2(445.0f, 188.0f));
		costIconObj->transform->SetSize(40.0f, 40.0f);
		costIconObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SHOP\\point.png");
		m_pages->AddObject(1, costIconObj);

		// コスト表示
		GameObject* costObj = new GameObject();
		costObj->SetParent(costIconObj);
		costObj->transform->SetPos(60.0f, 9.0f);
		costObj->AddComponent<CText>()->SetText(std::to_string(idx==0?m_fuelItem.GetPrice():m_enduranceItem.GetPrice()));
		costObj->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		costObj->GetComponent<CText>()->SetFontSize(36);
		costObj->GetComponent<CText>()->SetAlign(CText::CENTER);
		if (idx == 0) m_fuelCost = costObj;
		else m_enduranceCost = costObj;
		m_pages->AddObject(1, costObj);

		// 増加量表示
		GameObject* buttonTextObj = new GameObject();
		buttonTextObj->SetParent(buttonObj);
		buttonTextObj->transform->SetPos(195.0f, 23.0f);
		buttonTextObj->AddComponent<CText>()->SetText("+"+std::to_string(idx==0?m_fuelItem.GetAmount():m_enduranceItem.GetAmount()));
		buttonTextObj->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		buttonTextObj->GetComponent<CText>()->SetFontSize(60);
		buttonTextObj->GetComponent<CText>()->SetAlign(CText::CENTER);
		if (idx == 0) m_fuelButtonText = buttonTextObj;
		else m_enduranceButtonText = buttonTextObj;
		m_pages->AddObject(1, buttonTextObj);
	}
}

//=============================================================
// [ShopManager] 終了
//=============================================================
void ShopManager::Uninit()
{
	// アイテムリストとパークリストを破棄する
	for (auto itr = m_itemList.begin(); itr != m_itemList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
		}
	}
	m_itemList.clear();

	for (auto itr = m_perkList.begin(); itr != m_perkList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
		}
	}
	m_perkList.clear();

	// 音を破棄する
	AudioManager::GetInstance()->RemoveClip(m_buySE);
}

//=============================================================
// [ShopManager] 更新
//=============================================================
void ShopManager::Update()
{
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	// ポイントの更新
	if (m_viewPoints < m_points) m_viewPoints++;
	else if (m_viewPoints > m_points) m_viewPoints--;
	m_ptText->GetComponent<CText>()->SetText("<color=23,116,255>" + std::to_string(m_viewPoints));

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

//=============================================================
// [ShopManager] リセット
//=============================================================
void ShopManager::Reset()
{
	m_points = 30;
	ShopItem::SetSaleDiscount(0.0f);
}

//=============================================================
// [ShopManager] アイテムの登録
//=============================================================
void ShopManager::RegisterItemList()
{
	// アイテムの登録
	m_itemList.push_back(new ShopFuelTank);
	m_itemList.push_back(new ShopToolBox);
	m_itemList.push_back(new ShopExplosionBoost);
	m_itemList.push_back(new ShopTeruTeruBouzu);
	m_itemList.push_back(new ShopMyRoad);

	// パークの登録
}

//=============================================================
// [ShopManager] ポイントを支払う
//=============================================================
bool ShopManager::PayPoint(const int& cost)
{
	if (cost <= m_points)
	{ // 支払える時
		m_points -= cost;
		return true;
	}
	return false;
}

