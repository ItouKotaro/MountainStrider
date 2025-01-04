//=============================================================
//
// �ԗ��p�[�e�B�N�� [vehicle_particle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_particle.h"
#include "scripts/trajectory.h"

//=============================================================
// [VehicleParticle] ������
//=============================================================
void VehicleParticle::Init()
{
	// �o�C�N���擾����
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// �O�Ղ̒ǉ�
	m_trajectory = new GameObject();
	m_trajectory->AddComponent<CTrajectory>()->SetShow(true);
}

//=============================================================
// [VehicleParticle] �I��
//=============================================================
void VehicleParticle::Uninit()
{
}

//=============================================================
// [VehicleParticle] �X�V
//=============================================================
void VehicleParticle::Update()
{
	// �O�Ղ̍X�V
	UpdateTrajectory();
}

//=============================================================
// [VehicleParticle] �`��
//=============================================================
void VehicleParticle::Draw()
{
}

//=============================================================
// [VehicleParticle] �O�Ղ̍X�V
//=============================================================
void VehicleParticle::UpdateTrajectory()
{
	auto trajectory = m_trajectory->GetComponent<CTrajectory>();

	D3DXMATRIX mtx;
	mtx = transform->GetMatrix();
	D3DXVECTOR3 pos0, pos1;
	pos0 = { -0.5f, -1.0f, 7.0f };
	pos1 = { 0.5f, -1.0f, 7.0f };
	D3DXVec3TransformCoord(&pos0, &pos0, &mtx);
	D3DXVec3TransformCoord(&pos1, &pos1, &mtx);

	trajectory->AddTrajectory(pos0, pos1);
}
