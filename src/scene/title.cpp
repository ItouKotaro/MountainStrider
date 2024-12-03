//=============================================================
//
// タイトル [title.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "title.h"

#include "component/2d/text.h"
#include "component/2d/polygon.h"
#include "component/other/button.h"
#include "scene/game.h"
#include "renderer.h"

//=============================================================
// [CTitleScene] 初期化
//=============================================================
void CTitleScene::Init()
{
	// 背景
	GameObject* pBG = new GameObject;
	pBG->AddComponent<CPolygon>();
	pBG->GetComponent<CPolygon>()->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pBG->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));

	// テキスト
	GameObject* pLogo = new GameObject;
	pLogo->transform->SetSize(600.0f, 600.0f);
	pLogo->AddComponent<CPolygon>();
	pLogo->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\logo.png");
	pLogo->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH) / 2 - 300.0f, 50.0f);

	// ゲーム開始
	{
		GameObject* pStartButton = new GameObject("StartMountain");
		pStartButton->transform->SetSize(500.0f, 140.0f);
		pStartButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) - 400.0f, 800.0f);
		pStartButton->AddComponent<ButtonUI>();
		pStartButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pStartButton->GetComponent<ButtonUI>()->setClickEvent([]() { 
			static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->ResetGame();
			CSceneManager::GetInstance()->SetScene("game");
			});

		GameObject* pStartButtonText = new GameObject();
		pStartButtonText->SetParent(pStartButton);
		pStartButtonText->transform->SetPos(250.0f, 30.0f);
		pStartButtonText->AddComponent<CText>();
		pStartButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pStartButtonText->GetComponent<CText>()->SetText("<color=0,0,0>開始");
		pStartButtonText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	}

	// リスト追加
	{
		GameObject* pEndButton = new GameObject("EndMountain");
		pEndButton->transform->SetSize(500.0f, 140.0f);
		pEndButton->transform->SetPos((CRenderer::SCREEN_WIDTH / 2 - 250.0f) + 400.0f, 800.0f);
		pEndButton->AddComponent<ButtonUI>();
		pEndButton->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\RESULT\\button.png");
		pEndButton->GetComponent<ButtonUI>()->setClickEvent([]() { DestroyWindow(CRenderer::GetInstance()->GetHWND()); });

		GameObject* pEndButtonText = new GameObject();
		pEndButtonText->SetParent(pEndButton);
		pEndButtonText->transform->SetPos(250.0f, 30.0f);
		pEndButtonText->AddComponent<CText>();
		pEndButtonText->GetComponent<CText>()->SetAlign(CText::CENTER);
		pEndButtonText->GetComponent<CText>()->SetText("<color=0,0,0>終了");
		pEndButtonText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	}


	CameraRenderBuffer* renderBuff = CRenderer::GetInstance()->RegisterRenderBuffer<CameraRenderBuffer>("main");
}

//=============================================================
// [CTitleScene] 終了
//=============================================================
void CTitleScene::Uninit()
{
}

//=============================================================
// [CTitleScene] 更新
//=============================================================
void CTitleScene::Update()
{
}

//=============================================================
// [CTitleScene] 描画
//=============================================================
void CTitleScene::Draw()
{
}