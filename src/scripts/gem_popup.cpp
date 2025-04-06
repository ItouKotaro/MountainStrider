//=============================================================
//
// ジェムのポップアップ表示UI [gem_popup.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "gem_popup.h"
#include "manager.h"
#include "shop/shop.h"

const D3DXVECTOR2 GemPopupUI::BG_SIZE = { 500.0f, 150.0f };

//=============================================================
// [GemPopupUI] 初期化
//=============================================================
void GemPopupUI::Init()
{
	// 変数の初期化
	m_oldPoint = 0;
	m_showTime = 0.0f;

	// 背景
	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetTexture("data\\TEXTURE\\UI\\gem_popup.png");
	m_bg->SetParent(gameObject);
	m_bg->transform->SetSize(BG_SIZE);

	// テキスト
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetText("0");
	m_text->SetFontSize(120);
	m_text->SetFontColor(D3DCOLOR_RGBA(41, 123, 230, 255));
	m_text->SetOutlineColor(D3DCOLOR_RGBA(41, 123, 230, 255));
	m_text->SetFont("07鉄瓶ゴシック");
	m_text->SetAlign(CText::CENTER);
	m_text->transform->SetPos(300.0f, 35.0f);
}

//=============================================================
// [GemPopupUI] 終了
//=============================================================
void GemPopupUI::Uninit()
{
	if (m_bg != nullptr)
	{
		m_bg->Uninit();
		delete m_bg;
		m_bg = nullptr;
	}

	if (m_text != nullptr)
	{
		m_text->Uninit();
		delete m_text;
		m_text = nullptr;
	}
}

//=============================================================
// [GemPopupUI] 更新
//=============================================================
void GemPopupUI::Update()
{
	// ポイントに変動があったとき
	if (ShopManager::GetPoints() != m_oldPoint)
	{
		// 表示時間を設定する
		m_showTime = SHOW_TIME;

		// テキストを更新する
		m_text->SetText(std::to_string(ShopManager::GetPoints()));

		// 数値を更新する
		m_oldPoint = ShopManager::GetPoints();
	}

	// 表示
	m_showTime -= CManager::GetInstance()->GetDeltaTime();
	if (m_showTime > 0.0f)
	{ // 表示中
		transform->SetPos((float)CRenderer::SCREEN_WIDTH / 2.0f - GemPopupUI::BG_SIZE.x / 2.0f, transform->GetPosY() + (10.0f - transform->GetPosY()) * DAMPING);
	}
	else
	{ // 非表示中
		transform->SetPos((float)CRenderer::SCREEN_WIDTH / 2.0f - GemPopupUI::BG_SIZE.x / 2.0f, transform->GetPosY() + (-200.0f - transform->GetPosY()) * DAMPING);
	}

	m_bg->Update();
	m_text->Update();
}

//=============================================================
// [GemPopupUI] UI描画
//=============================================================
void GemPopupUI::DrawUI()
{
	m_bg->DrawUI();
	m_text->DrawUI();
}
