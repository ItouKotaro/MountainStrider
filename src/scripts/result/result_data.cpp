//=============================================================
//
// データ表示 [result_data.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_data.h"

#include "component/2d/polygon.h"
#include "component/2d/bar.h"
#include "component/2d/text.h"

const D3DXVECTOR2 ResultDataView::TITLE_BG_SIZE = { 340.0f, 65.0f };

//=============================================================
// [ResultDataView] 初期化
//=============================================================
void ResultDataView::Init()
{
	{
		// タイトル背景
		m_timeTitleBG = new GameObject("TimeTitleBG", "ResultData");
		m_timeTitleBG->SetParent(gameObject);
		m_timeTitleBG->AddComponent<CPolygon>();
		m_timeTitleBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(36, 39, 255, 255));
		m_timeTitleBG->transform->SetSize(TITLE_BG_SIZE);

		// タイトルテキスト
		m_timeTitleText = new GameObject("TimeTitleText", "ResultData");
		m_timeTitleText->SetParent(gameObject);
		m_timeTitleText->AddComponent<CText>();
		m_timeTitleText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_timeTitleText->GetComponent<CText>()->SetText("クリアタイム");
		m_timeTitleText->GetComponent<CText>()->SetFontSize(60);
		m_timeTitleText->transform->SetPos(13.0f, 8.0f, 0.0f);

		// データ背景
		m_timeDataBG = new GameObject("TimeDataBG", "ResultData");
		m_timeDataBG->SetParent(gameObject);
		m_timeDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_timeDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_timeDataBG->AddComponent<CPolygon>();
		m_timeDataBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(119, 126, 166, 255));

		// データテキスト
		m_timeDataText = new GameObject("TimeDataText", "ResultData");
		m_timeDataText->SetParent(gameObject);
		m_timeDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 10.0f);
		m_timeDataText->AddComponent<CText>();
		m_timeDataText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_timeDataText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_timeDataText->GetComponent<CText>()->SetFontSize(60);
		m_timeDataText->GetComponent<CText>()->SetText("1:56");
	}
}

//=============================================================
// [ResultDataView] 終了
//=============================================================
void ResultDataView::Uninit()
{

}

//=============================================================
// [ResultDataView] 更新
//=============================================================
void ResultDataView::Update()
{

}