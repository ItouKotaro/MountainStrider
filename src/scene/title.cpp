//=============================================================
//
// タイトル [title.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "title.h"

#include "component/2d/text.h"
#include "component/2d/polygon.h"
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
	pBG->transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);

	// テキスト
	GameObject* pText = new GameObject;
	pText->AddComponent<CText>();
	pText->GetComponent<CText>()->SetText("<color=0,0,0><size=200>ゲーム名");
	pText->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
	pText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 120.0f);

	// 選択肢を生成する
	for (int i = 0; i < SELECT::MAX; i++)
	{
		// 項目の表示名を格納する
		std::string sShowName;
		switch (i)
		{
		case SELECT::START:
			sShowName = "開始";
			break;
		case SELECT::EXIT:
			sShowName = "終了";
			break;
		}

		// オブジェクトを生成する
		m_pSelectObj[i] = new GameObject;
		m_pSelectObj[i]->AddComponent<CText>();
		m_pSelectObj[i]->GetComponent<CText>()->SetText("<color=0,0,0>" + sShowName);
		m_pSelectObj[i]->GetComponent<CText>()->SetAlign(CText::ALIGN::CENTER);
		m_pSelectObj[i]->transform->SetPos(CRenderer::SCREEN_WIDTH / 2, 600.0f + i * 200.0f);
	}
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
	// 選択の更新
	UpdateSelect();

	// 選択したときの処理
	if (INPUT_INSTANCE->onTrigger("space") ||
		INPUT_INSTANCE->onTrigger("enter"))
	{
		switch (m_select)
		{
		case CTitleScene::START:
			CSceneManager::GetInstance()->SetScene("game");
			break;
		case CTitleScene::EXIT:
			DestroyWindow(CRenderer::GetInstance()->GetHWND());
			break;
		}
	}
}

//=============================================================
// [CTitleScene] 描画
//=============================================================
void CTitleScene::Draw()
{
}


//=============================================================
// [CTitleScene] 選択の更新
//=============================================================
void CTitleScene::UpdateSelect()
{
	// 上選択
	if (INPUT_INSTANCE->onTrigger("w") ||
		INPUT_INSTANCE->onTrigger("up"))
	{
		if (m_select > 0)
		{
			m_select = static_cast<SELECT>(m_select - 1);
		}
	}

	// 下選択
	if (INPUT_INSTANCE->onTrigger("s") ||
		INPUT_INSTANCE->onTrigger("down"))
	{
		if (m_select < SELECT::MAX - 1)
		{
			m_select = static_cast<SELECT>(m_select + 1);
		}
	}

	// UIの更新
	for (int i = 0; i < SELECT::MAX; i++)
	{
		m_pSelectObj[i]->GetComponent<CText>()->SetAlpha(i == m_select ? 1.0f : 0.2f);
	}
}
