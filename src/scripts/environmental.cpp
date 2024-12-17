//=============================================================
//
// �� [environmental.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental.h"

#include "render/shadow_rb.h"
#include "component/3d/camera.h"

//=============================================================
// [EnvironmentalEffect] ������
//=============================================================
void EnvironmentalEffect::Init()
{
	// ���Ԃ����߂�
	TIME time = static_cast<TIME>(rand() % TIME::MAX);
	SetTime(time);

	// �V������߂�
	SetWeather(new WeatherClear());
}

//=============================================================
// [EnvironmentalEffect] �I��
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
// [EnvironmentalEffect] �X�V
//=============================================================
void EnvironmentalEffect::Update()
{
	// �V��̍X�V
	if (m_weather != nullptr)
	{
		m_weather->Update();
	}
}

//=============================================================
// [EnvironmentalEffect] ���Ԃ̐ݒ�
//=============================================================
void EnvironmentalEffect::SetTime(const TIME& time)
{
	// �J�����̎擾
	auto pCamera = GameObject::Find("Camera")->GetComponent<CCamera>();

	// �A���r�G���g�p�ϐ�
	D3DXVECTOR4 ambient = { 1.0f, 1.0f, 1.0f, 1.0f };

	// ���Ԃ̐ݒ�
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

	// �V�[���̈Â��𒲐�����
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetAmbient(ambient);

	// ���Ԃ�ݒ肷��
	m_time = time;
}

//=============================================================
// [EnvironmentalEffect] �V��̐ݒ�
//=============================================================
void EnvironmentalEffect::SetWeather(Weather* weather)
{
	m_weather = weather;
	m_weather->Init();
}