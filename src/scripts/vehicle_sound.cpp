//=============================================================
//
// �ԗ��̃T�E���h [vehicle_sound.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_sound.h"


//=============================================================
// [VehicleSound] ������
//=============================================================
void VehicleSound::Init()
{
	// �o�C�N�R���|�[�l���g���擾����
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// ���s��
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
// [VehicleSound] �I��
//=============================================================
void VehicleSound::Uninit()
{

}

//=============================================================
// [VehicleSound] �X�V
//=============================================================
void VehicleSound::Update()
{
	// ����ł��Ȃ��Ƃ�
	if (!m_vehicle->GetFlying())
	{
		// �A�N�Z���𓥂�ł���Ƃ�
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

	// ����
	if (m_runnningVolume > m_runnningVolumeLimit)
	{
		m_runnningVolume += (m_runnningVolumeLimit - m_runnningVolume) * 0.08f;
	}
	if (m_runnningPitch > m_runnningPitchLimit)
	{
		m_runnningPitch += (m_runnningPitchLimit - m_runnningPitch) * 0.08f;
	}

	// �ݒ肷��
	m_runningSE->SetVolume(m_runnningVolume);
	m_runningSE->SetPitch(m_runnningPitch);
}