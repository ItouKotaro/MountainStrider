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
	gameObject->AddComponent<AudioSource>();

	// 走行音
	m_runningSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\BIKE\\running00.wav", FMOD_2D | FMOD_LOOP_NORMAL);
	m_runnningVolume = 0.0f;
	m_runnningVolumeLimit = 5.0f;
	m_runnningPitch = 1.0f;
	m_runnningPitchLimit = 0.6f;
	gameObject->GetComponent<AudioSource>()->Play(m_runningSE);
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
	if (m_vehicle->GetGroundDistance() < 20.0f)
	{
		// アクセルを踏んでいるとき
		if (INPUT_INSTANCE->onInput("accel"))
		{
			if (m_runnningVolume < 0.5f) m_runnningVolume = 0.5f;
			m_runnningVolume += 0.03f + m_vehicle->GetSpeed() / 1000.0f;
			m_runnningVolumeLimit = 1.0f;
			m_runnningPitch += 0.005f;
			m_runnningPitchLimit = 0.8f + m_vehicle->GetSpeed() / 200.0f;
		}
		else
		{
			m_runnningVolume += 0.08f;
			m_runnningVolumeLimit = 0.5f;
			m_runnningPitchLimit = 0.6f;
		}
	}
	else
	{
		m_runnningVolumeLimit = 0.1f;
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
	gameObject->GetComponent<AudioSource>()->GetChannel()->setVolume(m_runnningVolume);
	gameObject->GetComponent<AudioSource>()->GetChannel()->setPitch(m_runnningPitch);
}