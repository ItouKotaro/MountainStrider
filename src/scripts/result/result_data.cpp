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

const D3DXVECTOR2 ResultDataView::TITLE_BG_SIZE = { 350.0f, 80.0f };
const float ResultDataView::SPACE_SIZE = 150.0f;

//=============================================================
// [ResultDataView] 初期化
//=============================================================
void ResultDataView::Init()
{
	// クリアタイム
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
		m_timeTitleText->transform->SetPos(20.0f, 15.0f, 0.0f);

		// データ背景
		m_timeDataBG = new GameObject("TimeDataBG", "ResultData");
		m_timeDataBG->SetParent(gameObject);
		m_timeDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_timeDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_timeDataBG->AddComponent<CPolygon>();
		m_timeDataBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(78, 90, 135, 255));

		// データテキスト
		m_timeDataText = new GameObject("TimeDataText", "ResultData");
		m_timeDataText->SetParent(gameObject);
		m_timeDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_timeDataText->AddComponent<CText>();
		m_timeDataText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_timeDataText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_timeDataText->GetComponent<CText>()->SetFontSize(60);
	}

	// 最高速度
	{
		// タイトル背景
		m_speedTitleBG = new GameObject("SpeedTitleBG", "ResultData");
		m_speedTitleBG->SetParent(gameObject);
		m_speedTitleBG->AddComponent<CPolygon>();
		m_speedTitleBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(252, 135, 18, 255));
		m_speedTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_speedTitleBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// タイトルテキスト
		m_speedTitleText = new GameObject("SpeedTitleText", "ResultData");
		m_speedTitleText->SetParent(gameObject);
		m_speedTitleText->AddComponent<CText>();
		m_speedTitleText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_speedTitleText->GetComponent<CText>()->SetText("最高速度");
		m_speedTitleText->GetComponent<CText>()->SetFontSize(60);
		m_speedTitleText->transform->SetPos(70.0f, 15.0f, 0.0f);
		m_speedTitleText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// データ背景
		m_speedDataBG = new GameObject("SpeedDataBG", "ResultData");
		m_speedDataBG->SetParent(gameObject);
		m_speedDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_speedDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_speedDataBG->AddComponent<CPolygon>();
		m_speedDataBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(176, 109, 81, 255));
		m_speedDataBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// データテキスト
		m_speedDataText = new GameObject("SpeedDataText", "ResultData");
		m_speedDataText->SetParent(gameObject);
		m_speedDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_speedDataText->AddComponent<CText>();
		m_speedDataText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_speedDataText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_speedDataText->GetComponent<CText>()->SetFontSize(60);
		m_speedDataText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);
	}

	// アクション
	{
		// タイトル背景
		m_actionTitleBG = new GameObject("ActionTitleBG", "ResultData");
		m_actionTitleBG->SetParent(gameObject);
		m_actionTitleBG->AddComponent<CPolygon>();
		m_actionTitleBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(178, 36, 255, 255));
		m_actionTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_actionTitleBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// タイトルテキスト
		m_actionTitleText = new GameObject("ActionTitleText", "ResultData");
		m_actionTitleText->SetParent(gameObject);
		m_actionTitleText->AddComponent<CText>();
		m_actionTitleText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_actionTitleText->GetComponent<CText>()->SetText("アクション");
		m_actionTitleText->GetComponent<CText>()->SetFontSize(60);
		m_actionTitleText->transform->SetPos(50.0f, 15.0f, 0.0f);
		m_actionTitleText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// データ背景
		m_actionDataBG = new GameObject("ActionDataBG", "ResultData");
		m_actionDataBG->SetParent(gameObject);
		m_actionDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_actionDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_actionDataBG->AddComponent<CPolygon>();
		m_actionDataBG->GetComponent<CPolygon>()->SetColor(D3DCOLOR_RGBA(92, 72, 110, 255));
		m_actionDataBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// データテキスト
		m_actionDataText = new GameObject("ActionDataText", "ResultData");
		m_actionDataText->SetParent(gameObject);
		m_actionDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_actionDataText->AddComponent<CText>();
		m_actionDataText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
		m_actionDataText->GetComponent<CText>()->SetAlign(CText::CENTER);
		m_actionDataText->GetComponent<CText>()->SetFontSize(60);
		m_actionDataText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);
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

//=============================================================
// [ResultDataView] 時間の設定
//=============================================================
void ResultDataView::SetTime(const int& time)
{
	// 分と秒
	int min = (time - time % 60) / 60;
	int sec = time % 60;

	// 時間の表示形式を変更する
	char* timeText = new char[128];
	sprintf(timeText, "%d:%02d", min, sec);

	// テキストを更新する
	m_timeDataText->GetComponent<CText>()->SetText(timeText);

	// 破棄
	delete[] timeText;
	timeText = nullptr;
}

//=============================================================
// [ResultDataView] 最高速度の設定
//=============================================================
void ResultDataView::SetHighSpeed(const int& kmh)
{
	// テキストを更新する
	m_speedDataText->GetComponent<CText>()->SetText(std::to_string(kmh) + "<size=40>km/h");
}

//=============================================================
// [ResultDataView] アクションの設定
//=============================================================
void ResultDataView::SetAction(const int& score)
{
	// テキストを更新する
	m_actionDataText->GetComponent<CText>()->SetText(std::to_string(score));
}