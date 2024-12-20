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
	m_runningSE->transform->Translate(300.0f, 50.0f, 0.0f);
	m_runningSE->gameObject->SetParent(gameObject);
	m_runningSE->LoadWav("data\\SOUND\\SE\\BIKE\\running00.wav");
	m_runningSE->SetVolume(100.0f);
	m_runningSE->Play();
	m_runningSE->SetLoop(true);
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
}