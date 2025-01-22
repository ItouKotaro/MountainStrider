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
	AudioEventDesc vehicleDesc = AudioManager::GetInstance()->GetEventDesc("event:/Vehicles/Ride-on Mower");

	m_soundObj = new GameObject();
	m_soundObj->SetParent(gameObject);
	m_soundObj->AddComponent<AudioEvent>()->SetEventDesc(vehicleDesc);
	m_vehicleSound = m_soundObj->GetComponent<AudioEvent>()->GetInstance();
	m_vehicleSound->setVolume(2.0f);
	m_vehicleSound->setParameterByName("RPM", 10.0f);
	m_vehicleSound->start();
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
	if (m_vehicle->GetGroundDistance() < 20.0f)
	{
		// �A�N�Z���𓥂�ł���Ƃ�
		if (INPUT_INSTANCE->onInput("accel"))
		{
			if (m_vehicleRPM < 100.0f) m_vehicleRPM = 100.0f;
			m_vehicleRPM += 2.5f + m_vehicle->GetSpeed() / 10.0f;
			m_vehicleRPMLimit = 800.0f;
		}
		else
		{
			m_vehicleRPM -= 4.0f;
			m_vehicleRPMLimit = 600.0f;
		}
	}
	else
	{
		m_vehicleRPMLimit = 300.0f;
	}

	m_vehicleRPM -= 2.0f;
	m_vehicleRPM += m_vehicle->GetSpeed() * 0.1f;

	// ����
	if (m_vehicleRPM > m_vehicleRPMLimit)
	{
		m_vehicleRPM += (m_vehicleRPMLimit - m_vehicleRPM) * 0.08f;
	}

	m_vehicleSound->setParameterByName("RPM", m_vehicleRPM);
}