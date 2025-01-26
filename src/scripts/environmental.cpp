//=============================================================
//
// 環境 [environmental.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental.h"

#include "render/shadow_rb.h"
#include "scripts/result/mt_result.h"
#include <fstream>

//=============================================================
// [EnvironmentalEffect] 初期化
//=============================================================
void EnvironmentalEffect::Init(const std::string path)
{
	// ライトを生成する
	m_light = new GameObject("Light");
	m_light->AddComponent<CCamera>();
	m_light->GetComponent<CCamera>()->m_fClippingPlanesFar = 5000.0f;

	// バイクを取得する
	m_vehicle = GameObject::Find("Vehicle");

	// 時間を決める
	TIME time = static_cast<TIME>(rand() % TIME::MAX);
	if (ResultBase::GetNumOfStep() == 0)
	{ // 初回
		time = TIME::MORNING;
	}
	SetTime(time);

	// 天候を決める
	Weather* weather;
	if (ResultBase::GetNumOfStep() == 0)
	{ // 初回
		weather = new WeatherClear();
	}
	else
	{
		int weatherRand = rand() % 2;
		if (weatherRand == 0) weather = new WeatherClear();
		else weather = new WeatherRain();
	}
	SetWeather(weather);


	// BGMを設定する --------------------------------------------------------------
	std::ifstream ifs(path.c_str());
	if (ifs.fail())
	{ // ファイルの読み込みに失敗
		return;
	}

	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jInput = json::parse(sInputData);

	// BGMを読み込む
	if (jInput.contains("music"))
	{
		m_bgm = new GameObject("BGM");
		m_bgm->AddComponent<AudioSource>();

		m_bgmClip = nullptr;
		float volume = 1.0f;

		// 朝
		if (m_time == TIME::MORNING && jInput["music"].contains("morning"))
		{
			if (jInput["music"]["morning"].contains("volume")) volume = jInput["music"]["morning"]["volume"];
			m_bgmClip = AudioManager::GetInstance()->CreateClip(jInput["music"]["morning"]["path"], FMOD_2D | FMOD_LOOP_NORMAL, true);
		}

		// 昼
		if (m_time == TIME::DAYTIME && jInput["music"].contains("daytime"))
		{
			if (jInput["music"]["daytime"].contains("volume")) volume = jInput["music"]["daytime"]["volume"];
			m_bgmClip = AudioManager::GetInstance()->CreateClip(jInput["music"]["daytime"]["path"], FMOD_2D | FMOD_LOOP_NORMAL, true);
		}

		// 夜
		if (m_time == TIME::NIGHT && jInput["music"].contains("night"))
		{
			if (jInput["music"]["night"].contains("volume")) volume = jInput["music"]["night"]["volume"];
			m_bgmClip = AudioManager::GetInstance()->CreateClip(jInput["music"]["night"]["path"], FMOD_2D | FMOD_LOOP_NORMAL, true);
		}

		// 再生する
		m_bgm->GetComponent<AudioSource>()->Play(m_bgmClip);
		m_bgm->GetComponent<AudioSource>()->GetChannel()->setVolume(volume);
	}

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
		m_weather = nullptr;
	}

	// BGMを破棄する
	m_bgm->Destroy();
	m_bgm = nullptr;
	AudioManager::GetInstance()->RemoveClip(m_bgmClip);
	m_bgmClip = nullptr;
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
	if (m_weather != nullptr)
	{
		m_weather->Uninit();
		delete m_weather;
		m_weather = nullptr;
	}

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