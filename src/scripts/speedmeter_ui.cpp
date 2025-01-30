//=============================================================
//
// スピードメーターUI [speedmeter_ui.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "speedmeter_ui.h"
#include "renderer.h"

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
	m_speedText->SetFontSize(50);
	m_speedText->SetAlign(CText::CENTER);
	m_speedText->transform->SetPos(160.0f, 250.0f);

	// スピードテキストの背景
	m_speedBG = new SingleComponent<CPolygon>();
	m_speedBG->Init();
	m_speedBG->SetParent(gameObject);
	m_speedBG->SetColor(D3DCOLOR_RGBA(42, 71, 184, 150));
	m_speedBG->transform->SetPos(70.0f, 240.0f);
	m_speedBG->transform->SetSize(162.5f, 60.0f);

	// メーターの生成
	m_meterBG = new SingleComponent<CPolygon>();
	m_meterBG->Init();
	m_meterBG->SetParent(gameObject);
	m_meterBG->SetTexture("data\\TEXTURE\\UI\\meter.png");
	m_meterBG->transform->SetSize(300.0f, 300.0f);

	// 針の軸
	m_showAngle = D3DX_PI * 0.27f;
	m_meter = new Transform();
	m_meter->SetParent(gameObject->transform);
	m_meter->SetPos(150.0f, 170.0f);
	m_meter->SetRot(m_showAngle);

	// 針の生成
	m_needle = new SingleComponent<CPolygon>();
	m_needle->Init();
	m_needle->SetParent(m_meter);
	m_needle->SetTexture("data\\TEXTURE\\UI\\needle.png");
	m_needle->transform->SetPos(-10.0f, -10.0f);
	m_needle->transform->SetSize(20.0f, 150.0f);
}

//=============================================================
// [SpeedMeterUI] 終了
//=============================================================
void SpeedMeterUI::Uninit()
{
	m_speedText->Uninit();
	delete m_speedText;

	m_speedBG->Uninit();
	delete m_speedBG;

	m_meterBG->Uninit();
	delete m_meterBG;

	m_needle->Uninit();
	delete m_needle;

	delete m_meter;
}

//=============================================================
// [SpeedMeterUI] 更新
//=============================================================
void SpeedMeterUI::Update()
{
	// スピードテキストを更新する
	char cSpeedText[24];
	sprintf(&cSpeedText[0], "%.1f<size=20>Km/h", m_vehicle->GetSpeed());
	m_speedText->SetText(cSpeedText);

	// メーターの更新
	UpdateMeter();

	// 更新
	m_speedText->Update();
	m_speedBG->Update();
	m_meterBG->Update();
	m_needle->Update();
}

//=============================================================
// [SpeedMeterUI] メーターの更新
//=============================================================
void SpeedMeterUI::UpdateMeter()
{
	// スピードから何個までバーを転倒するかを決める
	float speed = m_vehicle->GetSpeed();
	if (speed > 260.0f) { speed = 260.0f; }

	m_showAngle += ((D3DX_PI * (0.27f + 0.56f * (speed / (float)100.0f))) - m_showAngle) * 0.08f;
	m_meter->SetRot(m_showAngle);
}

//=============================================================
// [SpeedMeterUI] UI描画
//=============================================================
void SpeedMeterUI::DrawUI()
{
	// 描画
	m_speedBG->DrawUI();
	m_meterBG->DrawUI();
	m_speedText->DrawUI();
	m_needle->DrawUI();
}