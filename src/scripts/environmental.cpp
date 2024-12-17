//=============================================================
//
// 環境 [environmental.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental.h"

#include "render/shadow_rb.h"
#include "component/3d/camera.h"

//=============================================================
// [EnvironmentalEffect] 初期化
//=============================================================
void EnvironmentalEffect::Init()
{
	// 時間を決める
	TIME time = static_cast<TIME>(rand() % TIME::MAX);
	SetTime(time);

	// 天候を決める
	SetWeather(new WeatherClear());
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