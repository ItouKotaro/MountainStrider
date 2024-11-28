//=============================================================
//
// リザルトビュー [result_view.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "result_view.h"

#include "component/2d/bar.h"
#include "component/2d/text.h"

const int ResultViewBar::ANIM_FRAME = 60;

//=============================================================
// [ResultViewBar] コンストラクタ
//=============================================================
ResultViewBar::ResultViewBar(const std::string& title, const D3DXCOLOR& titleColor, const D3DXCOLOR& barColor, const D3DXCOLOR& percentColor)
{
	m_titleName = title;
	m_titleColor = titleColor;
	m_barColor = barColor;
	m_percentColor = percentColor;
}

//=============================================================
// [ResultViewBar] 初期化
//=============================================================
void ResultViewBar::Init()
{
	// アニメーションの初期化
	m_isStartAnim = false;
	m_animCounter = 0;

	// タイトルテキスト
	m_viewText = new GameObject("TitleText", "UI");
	m_viewText->SetParent(gameObject);
	m_viewText->AddComponent<CText>();
	m_viewText->GetComponent<CText>()->SetFontSize(60);
	m_viewText->GetComponent<CText>()->SetOutlineSize(2);
	m_viewText->GetComponent<CText>()->SetFontColor(m_titleColor);
	m_viewText->GetComponent<CText>()->SetOutlineColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_viewText->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_viewText->GetComponent<CText>()->SetText(m_titleName);

	// バー
	m_viewBar = new GameObject("Bar", "UI");
	m_viewBar->SetParent(gameObject);
	m_viewBar->transform->SetPos(0.0f, 80.0f);
	m_viewBar->AddComponent<CAdvancedBar>();
	m_viewBar->GetComponent<CAdvancedBar>()->SetTexture("data\\TEXTURE\\RESULT\\hand_bar.png");
	m_viewBar->GetComponent<CAdvancedBar>()->SetProgress(1.0f);
	m_viewBar->GetComponent<CAdvancedBar>()->SetBGColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_viewBar->GetComponent<CAdvancedBar>()->SetLength(500.0f);
	m_viewBar->GetComponent<CAdvancedBar>()->SetBold(35.0f);
	m_viewBar->GetComponent<CAdvancedBar>()->SetColor(0, m_barColor);
	m_viewBar->GetComponent<CAdvancedBar>()->SetColor(2, m_barColor);
	m_viewBar->GetComponent<CAdvancedBar>()->SetColor(1, m_barColor);
	m_viewBar->GetComponent<CAdvancedBar>()->SetColor(3, m_barColor);

	// 割合テキスト
	m_viewValue = new GameObject("Value", "UI");
	m_viewValue->SetParent(gameObject);
	m_viewValue->transform->SetPos(500.0f, 10.0f);
	m_viewValue->AddComponent<CText>();
	m_viewValue->GetComponent<CText>()->SetAlign(CText::RIGHT);
	m_viewValue->GetComponent<CText>()->SetFontSize(60);
	m_viewValue->GetComponent<CText>()->SetOutlineSize(2);
	m_viewValue->GetComponent<CText>()->SetFontColor(m_percentColor);
	m_viewValue->GetComponent<CText>()->SetOutlineColor(D3DCOLOR_RGBA(230, 230, 230, 255));
	m_viewValue->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_viewValue->GetComponent<CText>()->SetText("100%");

	// 比較テキスト
	m_viewCompare = new GameObject("Compare", "UI");
	m_viewCompare->SetParent(gameObject);
	m_viewCompare->transform->SetPos(500.0f, 110.0f);
	m_viewCompare->AddComponent<CText>();
	m_viewCompare->GetComponent<CText>()->SetAlign(CText::RIGHT);
	m_viewCompare->GetComponent<CText>()->SetFontSize(30);
	m_viewCompare->GetComponent<CText>()->SetFont("07鉄瓶ゴシック");
	m_viewCompare->GetComponent<CText>()->SetText("<color=230,230,230>-0.0%");
}

//=============================================================
// [ResultViewBar] 終了
//=============================================================
void ResultViewBar::Uninit()
{

}

//=============================================================
// [ResultViewBar] 更新
//=============================================================
void ResultViewBar::Update()
{
	if (!m_isStartAnim)
	{
		return;	// ストップの時は処理を飛ばす
	}

	// 現在の進行度を計算する
	m_viewProgress = m_viewBefore + ((m_viewObjective - m_viewBefore) / ANIM_FRAME) * (m_animCounter+1);

	// 進行度を更新する
	m_viewBar->GetComponent<CAdvancedBar>()->SetProgress(m_viewProgress);
	m_viewValue->GetComponent<CText>()->SetText(std::to_string(static_cast<int>(m_viewProgress * 100.0f))+"%");

	// 前回と比較してどうか
	float compareView = (m_viewProgress - m_viewBefore) * 100;
	char* compareText = new char[24];
	sprintf(compareText, "%.1f", fabs(compareView));

	std::string mark = "<color=230,230,230>-";
	if (compareView <= -0.1f)
	{ // 下がっている
		mark = "<color=31,120,255>↓";
	}
	else if (compareView >= 0.1f)
	{ // 上がっている
		mark = "<color=255,121,25>↑";
	}
	m_viewCompare->GetComponent<CText>()->SetText(mark + compareText + "%");
	
	// 解放
	delete[] compareText;

	// カウンターを進める
	m_animCounter++;
	if (m_animCounter >= ANIM_FRAME)
	{
		m_animCounter = 0;
		m_isStartAnim = false;
	}
}

//=============================================================
// [ResultViewBar] 値の設定
//=============================================================
void ResultViewBar::SetValue(const float& before, const float& objective)
{
	m_viewBefore = before;
	m_viewObjective = objective;
	m_viewProgress = before;

	m_viewBar->GetComponent<CAdvancedBar>()->SetProgress(m_viewBefore);
	m_viewValue->GetComponent<CText>()->SetText(std::to_string(static_cast<int>(m_viewProgress * 100.0f)) + "%");
}