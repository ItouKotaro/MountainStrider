//=============================================================
//
// 環境 [environmental.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental.h"

#include "render/shadow_rb.h"

//=============================================================
// [EnvironmentalEffect] 初期化
//=============================================================
void EnvironmentalEffect::Init()
{
	// ライトを生成する
	m_light = new GameObject("Light");
	m_light->AddComponent<CCamera>();
	m_light->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;

	// バイクを取得する
	m_vehicle = GameObject::Find("Vehicle");

	// 時間を決める
	TIME time = static_cast<TIME>(rand() % TIME::MAX);
	SetTime(time);

	// 天候を決める
	Weather* weather;
	int weatherRand = rand() % 2;
	if (weatherRand == 0) weather = new WeatherClear();
	else weather = new WeatherRain();

	SetWeather(weather);
}

//=============================================================
// [EnvironmentalEffect] 終了
//=============================================================
void EnvironmentalEffect::Uninit()
{
	if (m_weather != nullptr)
	{
		m_weather->Uninit();
		delete m_weather;
	}
}

//=============================================================
// [EnvironmentalEffect] 更新
//=============================================================
void EnvironmentalEffect::Update()
{
	// 天候の更新
	if (m_weather != nullptr)
	{
		m_weather->Update();
	}

	// ライトの更新
	UpdateLight();
}

//=============================================================
// [EnvironmentalEffect] 時間の設定
//=============================================================
void EnvironmentalEffect::SetTime(const TIME& time)
{
	// カメラの取得
	auto pCamera = GameObject::Find("Camera")->GetComponent<CCamera>();

	// アンビエント用変数
	D3DXVECTOR4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 時間の設定
	switch (time)
	{
	case TIME::MORNING:
		pCamera->GetSkybox()->LoadSkybox("data\\SKYBOX\\daylight00.json");
		ambient = { 0.6f, 0.6f, 0.6f, 1.0f };
		break;
	case TIME::DAYTIME:
		pCamera->GetSkybox()->LoadSkybox("data\\SKYBOX\\daylight01.json");
		ambient = { 0.8f, 0.8f, 0.8f, 1.0f };
		break;
	case TIME::NIGHT:
		pCamera->GetSkybox()->LoadSkybox("data\\SKYBOX\\darknight00.json");
		ambient = { 0.3f, 0.3f, 0.3f, 1.0f };
		break;
	}

	// シーンの暗さを調整する
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetAmbient(ambient);

	// 時間を設定する
	m_time = time;
}

//=============================================================
// [EnvironmentalEffect] 天候の設定
//=============================================================
void EnvironmentalEffect::SetWeather(Weather* weather)
{
	m_weather = weather;
	m_weather->Init();
}

//=============================================================
// [EnvironmentalEffect] ライト位置の更新
//=============================================================
void EnvironmentalEffect::UpdateLight()
{
	D3DXVECTOR3 pos = GameObject::Find("Camera")->transform->GetWPos();
	float angle = Benlib::PosAngle(pos, m_vehicle->transform->GetWPos());

	// 時間の設定
	switch (m_time)
	{
	case TIME::MORNING:
		m_light->transform->SetPos(m_vehicle->transform->GetWPos() + D3DXVECTOR3(300.0f, 700.0f, -100.0f));
		m_light->transform->LookAt(m_vehicle->transform->GetWPos());
		m_light->transform->Translate(sinf(angle) * 200.0f, 0.0f, cosf(angle) * 200.0f);
		static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowPoint(
			m_vehicle->transform->GetWPos() + D3DXVECTOR3(sinf(angle) * 200.0f, 0.0f, cosf(angle) * 200.0f)
		);
		static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowRange(350.0f);
		break;
	case TIME::DAYTIME:
		m_light->transform->SetPos(m_vehicle->transform->GetWPos() + D3DXVECTOR3(300.0f, 700.0f, -100.0f));
		m_light->transform->LookAt(m_vehicle->transform->GetWPos());
		m_light->transform->Translate(sinf(angle) * 200.0f, 0.0f, cosf(angle) * 200.0f);
		static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowPoint(
			m_vehicle->transform->GetWPos() + D3DXVECTOR3(sinf(angle) * 200.0f, 0.0f, cosf(angle) * 200.0f)
		);
		static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowRange(350.0f);
		break;
	case TIME::NIGHT:
		m_light->SetParent(m_vehicle);
		m_light->transform->SetPos(0.0f, 15.0f, -25.0f);
		m_light->transform->SetRot(0.15f, D3DX_PI, 0.0f);
		static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowPoint(
			m_light->transform->GetWPos() + D3DXVECTOR3(sinf(m_vehicle->transform->GetWRot().y) * -250.0f, 0.0f, cosf(m_vehicle->transform->GetWRot().y) * -250.0f)
		);
		static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetShadowRange(500.0f);
		break;
	}
}