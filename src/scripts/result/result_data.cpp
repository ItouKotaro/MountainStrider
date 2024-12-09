//=============================================================
//
// データ表示 [result_data.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_data.h"

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
		m_timeTitleBG = new SingleComponent<CPolygon>();
		m_timeTitleBG->Init();
		m_timeTitleBG->SetParent(gameObject);
		m_timeTitleBG->SetColor(D3DCOLOR_RGBA(36, 39, 255, 255));
		m_timeTitleBG->transform->SetSize(TITLE_BG_SIZE);

		// タイトルテキスト
		m_timeTitleText = new SingleComponent<CText>();
		m_timeTitleText->Init();
		m_timeTitleText->SetParent(gameObject);
		m_timeTitleText->SetFont("07鉄瓶ゴシック");
		m_timeTitleText->SetText("クリアタイム");
		m_timeTitleText->SetFontSize(60);
		m_timeTitleText->transform->SetPos(20.0f, 15.0f, 0.0f);

		// データ背景
		m_timeDataBG = new SingleComponent<CPolygon>();
		m_timeDataBG->Init();
		m_timeDataBG->SetParent(gameObject);
		m_timeDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_timeDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_timeDataBG->SetColor(D3DCOLOR_RGBA(78, 90, 135, 255));

		// データテキスト
		m_timeDataText = new SingleComponent<CText>();
		m_timeDataText->Init();
		m_timeDataText->SetParent(gameObject);
		m_timeDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_timeDataText->SetFont("07鉄瓶ゴシック");
		m_timeDataText->SetAlign(CText::CENTER);
		m_timeDataText->SetFontSize(60);
	}

	// 最高速度
	{
		// タイトル背景
		m_speedTitleBG = new SingleComponent<CPolygon>();
		m_speedTitleBG->Init();
		m_speedTitleBG->SetParent(gameObject);
		m_speedTitleBG->SetColor(D3DCOLOR_RGBA(252, 135, 18, 255));
		m_speedTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_speedTitleBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// タイトルテキスト
		m_speedTitleText = new SingleComponent<CText>();
		m_speedTitleText->Init();
		m_speedTitleText->SetParent(gameObject);
		m_speedTitleText->SetFont("07鉄瓶ゴシック");
		m_speedTitleText->SetText("最高速度");
		m_speedTitleText->SetFontSize(60);
		m_speedTitleText->transform->SetPos(70.0f, 15.0f, 0.0f);
		m_speedTitleText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// データ背景
		m_speedDataBG = new SingleComponent<CPolygon>();
		m_speedDataBG->Init();
		m_speedDataBG->SetParent(gameObject);
		m_speedDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_speedDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_speedDataBG->SetColor(D3DCOLOR_RGBA(176, 109, 81, 255));
		m_speedDataBG->transform->Translate(0.0f, SPACE_SIZE, 0.0f);

		// データテキスト
		m_speedDataText = new SingleComponent<CText>();
		m_speedDataText->Init();
		m_speedDataText->SetParent(gameObject);
		m_speedDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_speedDataText->SetFont("07鉄瓶ゴシック");
		m_speedDataText->SetAlign(CText::CENTER);
		m_speedDataText->SetFontSize(60);
		m_speedDataText->transform->Translate(0.0f, SPACE_SIZE, 0.0f);
	}

	// アクション
	{
		// タイトル背景
		m_actionTitleBG = new SingleComponent<CPolygon>();
		m_actionTitleBG->Init();
		m_actionTitleBG->SetParent(gameObject);
		m_actionTitleBG->SetColor(D3DCOLOR_RGBA(178, 36, 255, 255));
		m_actionTitleBG->transform->SetSize(TITLE_BG_SIZE);
		m_actionTitleBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// タイトルテキスト
		m_actionTitleText = new SingleComponent<CText>();
		m_actionTitleText->Init();
		m_actionTitleText->SetParent(gameObject);
		m_actionTitleText->SetFont("07鉄瓶ゴシック");
		m_actionTitleText->SetText("アクション");
		m_actionTitleText->SetFontSize(60);
		m_actionTitleText->transform->SetPos(50.0f, 15.0f, 0.0f);
		m_actionTitleText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// データ背景
		m_actionDataBG = new SingleComponent<CPolygon>();
		m_actionDataBG->Init();
		m_actionDataBG->SetParent(gameObject);
		m_actionDataBG->transform->SetPos(TITLE_BG_SIZE.x, 0.0f);
		m_actionDataBG->transform->SetSize(250.0f, TITLE_BG_SIZE.y);
		m_actionDataBG->SetColor(D3DCOLOR_RGBA(92, 72, 110, 255));
		m_actionDataBG->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);

		// データテキスト
		m_actionDataText = new SingleComponent<CText>();
		m_actionDataText->Init();
		m_actionDataText->SetParent(gameObject);
		m_actionDataText->transform->SetPos(TITLE_BG_SIZE.x + 250.0f / 2, 18.0f);
		m_actionDataText->SetFont("07鉄瓶ゴシック");
		m_actionDataText->SetAlign(CText::CENTER);
		m_actionDataText->SetFontSize(60);
		m_actionDataText->transform->Translate(0.0f, SPACE_SIZE * 2, 0.0f);
	}
}

//=============================================================
// [ResultDataView] 終了
//=============================================================
void ResultDataView::Uninit()
{
	m_timeTitleBG->Uninit();
	delete m_timeTitleBG;

	m_timeTitleText->Uninit();
	delete m_timeTitleText;

	m_timeDataBG->Uninit();
	delete m_timeDataBG;

	m_timeDataText->Uninit();
	delete m_timeDataText;

	m_speedTitleBG->Uninit();
	delete m_speedTitleBG;

	m_speedTitleText->Uninit();
	delete  m_speedTitleText;

	m_speedDataBG->Uninit();
	delete m_speedDataBG;

	m_speedDataText->Uninit();
	delete m_speedDataText;

	m_actionTitleBG->Uninit();
	delete m_actionTitleBG;

	m_actionTitleText->Uninit();
	delete m_actionTitleText;

	m_actionDataBG->Uninit();
	delete m_actionDataBG;

	m_actionDataText->Uninit();
	delete m_actionDataText;
}

//=============================================================
// [ResultDataView] 更新
//=============================================================
void ResultDataView::Update()
{
	m_timeTitleBG->Update();
	m_timeTitleText->Update();
	m_timeDataBG->Update();
	m_timeDataText->Update();

	m_speedTitleBG->Update();
	m_speedTitleText->Update();
	m_speedDataBG->Update();
	m_speedDataText->Update();

	m_actionTitleBG->Update();
	m_actionTitleText->Update();
	m_actionDataBG->Update();
	m_actionDataText->Update();
}

//=============================================================
// [ResultDataView] 描画
//=============================================================
void ResultDataView::DrawUI()
{
	m_timeTitleBG->DrawUI();
	m_timeTitleText->DrawUI();
	m_timeDataBG->DrawUI();
	m_timeDataText->DrawUI();

	m_speedTitleBG->DrawUI();
	m_speedTitleText->DrawUI();
	m_speedDataBG->DrawUI();
	m_speedDataText->DrawUI();

	m_actionTitleBG->DrawUI();
	m_actionTitleText->DrawUI();
	m_actionDataBG->DrawUI();
	m_actionDataText->DrawUI();
}

//=============================================================
// [ResultDataView] 時間の設定
//=============================================================
void ResultDataView::SetTime(const int& time)
{
	if (time == -1)
	{
		m_timeDataText->SetText("なし");
		return;
	}

	// 分と秒
	int min = (time - time % 60) / 60;
	int sec = time % 60;

	// 時間の表示形式を変更する
	char* timeText = new char[128];
	sprintf(timeText, "%d:%02d", min, sec);

	// テキストを更新する
	m_timeDataText->SetText(timeText);

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
	m_speedDataText->SetText(std::to_string(kmh) + "<size=40>km/h");
}

//=============================================================
// [ResultDataView] アクションの設定
//=============================================================
void ResultDataView::SetAction(const int& score)
{
	// テキストを更新する
	m_actionDataText->SetText(std::to_string(score));
}