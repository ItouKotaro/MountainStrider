//=============================================================
//
// スピードメーターUI [speedmeter_ui.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "speedmeter_ui.h"


//=============================================================
// [SpeedMeterUI] 初期化
//=============================================================
void SpeedMeterUI::Init()
{
	// スピードテキストの初期化
	m_speedText = new SingleComponent<CText>();
	m_speedText->Init();
	m_speedText->SetParent(gameObject);
	m_speedText->SetFont("07鉄瓶ゴシック");
}

//=============================================================
// [SpeedMeterUI] 終了
//=============================================================
void SpeedMeterUI::Uninit()
{
	m_speedText->Uninit();
	delete m_speedText;
}

//=============================================================
// [SpeedMeterUI] 更新
//=============================================================
void SpeedMeterUI::Update()
{
	// スピードテキストを更新する
	char cSpeedText[12];
	sprintf(&cSpeedText[0], "%.1fKm/h", m_vehicle->GetSpeed());
	m_speedText->SetText(cSpeedText);


	// 更新
	m_speedText->Update();
}

//=============================================================
// [SpeedMeterUI] UI描画
//=============================================================
void SpeedMeterUI::DrawUI()
{
	// 描画
	m_speedText->DrawUI();
}