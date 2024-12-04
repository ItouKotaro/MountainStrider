//=============================================================
//
// ショップ [shop.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "shop.h"

#include "renderer.h"
#include "component/2d/text.h"

//=============================================================
// [ShopManager] 初期化
//=============================================================
void ShopManager::Init()
{
	m_text = new GameObject();
	m_text->AddComponent<CText>()->SetText("ショップ");

	m_ptView = new PointView();
	m_ptView->Init();
}

//=============================================================
// [ShopManager] 終了
//=============================================================
void ShopManager::Uninit()
{
	m_ptView->Uninit();
	delete m_ptView;
}

//=============================================================
// [ShopManager] 更新
//=============================================================
void ShopManager::Update()
{
	m_ptView->Update();
}

//=============================================================
// [ShopManager] 描画
//=============================================================
void ShopManager::Draw()
{
	m_ptView->DrawUI();
}


//=============================================================
// [PointView] 初期化
//=============================================================
void PointView::Init()
{
	// ポイントフレームの初期化
	m_pointFrame = new CPolygon();
	m_pointFrame->transform = new Transform();
	m_pointFrame->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
	m_pointFrame->Init();
	m_pointFrame->SetTexture("data\\TEXTURE\\SHOP\\shop.png");
	

}

//=============================================================
// [PointView] 終了
//=============================================================
void PointView::Uninit()
{
	// ポイントフレームの終了
	if (m_pointFrame != nullptr)
	{
		m_pointFrame->Uninit();
		delete m_pointFrame->transform;
		delete m_pointFrame;
		m_pointFrame = nullptr;
	}
}

//=============================================================
// [PointView] 更新
//=============================================================
void PointView::Update()
{
	// ポイントフレームの更新
	m_pointFrame->Update();
}

//=============================================================
// [PointView] 描画
//=============================================================
void PointView::DrawUI()
{
	// ポイントフレームの描画
	m_pointFrame->DrawUI();
}