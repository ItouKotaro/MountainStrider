//=============================================================
//
// 車両のサウンド [vehicle_sound.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_sound.h"


//=============================================================
// [VehicleSound] 初期化
//=============================================================
void VehicleSound::Init()
{
	// バイクコンポーネントを取得する
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// 走行音
	m_runningSE = (new GameObject())->AddComponent<CSound>();
	m_runningSE->gameObject->SetParent(gameObject);
	m_runningSE->LoadWav("data\\SOUND\\SE\\BIKE\\running00.wav");
	m_runningSE->SetVolume(0.0f);
	m_runningSE->Play();
	m_runningSE->SetLoop(true);
	m_runnningVolume = 0.0f;
	m_runnningVolumeLimit = 100.0f;
	m_runnningPitch = 1.0f;
	m_runnningPitchLimit = 1.0f;
}

//=============================================================
// [VehicleSound] 終了
//=============================================================
void VehicleSound::Uninit()
{

}

//=============================================================
// [VehicleSound] 更新
//=============================================================
void VehicleSound::Update()
{
	// 空飛んでいないとき
	if (!m_vehicle->GetFlying())
	{
		// アクセルを踏んでいるとき
		if (INPUT_INSTANCE->onInput("accel"))
		{
			if (m_runnningVolume < 50.0f) m_runnningVolume = 50.0f;
			m_runnningVolume += 2.0f + m_vehicle->GetSpeed() / 100.0f;
			m_runnningVolumeLimit = 250.0f;
			m_runnningPitch += 0.02f;
			m_runnningPitchLimit = 1.55f;
		}
		else
		{
			m_runnningVolume += 1.0f;
			m_runnningVolumeLimit = 120.0f;
			m_runnningPitchLimit = 1.0f;
		}
	}
	else
	{
		m_runnningVolume -= 1.0f;
		m_runnningVolumeLimit = 90.0f;
		m_runnningPitchLimit = 1.0f;
	}

	// 制限
	if (m_runnningVolume > m_runnningVolumeLimit)
	{
		m_runnningVolume += (m_runnningVolumeLimit - m_runnningVolume) * 0.08f;
	}
	if (m_runnningPitch > m_runnningPitchLimit)
	{
		m_runnningPitch += (m_runnningPitchLimit - m_runnningPitch) * 0.08f;
	}

	// 設定する
	m_runningSE->SetVolume(m_runnningVolume);
	m_runningSE->SetPitch(m_runnningPitch);
}