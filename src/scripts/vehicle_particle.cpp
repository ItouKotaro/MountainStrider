//=============================================================
//
// �ԗ��p�[�e�B�N�� [vehicle_particle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_particle.h"

//=============================================================
// [VehicleParticle] ������
//=============================================================
void VehicleParticle::Init()
{
	// �o�C�N���擾����
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// �p�[�e�B�N���V�X�e���𐶐�����
	m_particleSystem = new SingleComponent<ParticleSystem>();
	m_particleSystem->SetParent(gameObject);
	m_particleSystem->Init();
	m_particleSystem->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\particle00.png");

	ParticleShape::PointShape* pointShape = new ParticleShape::PointShape();
	m_particleSystem->SetShape(pointShape);

	ParticleModule::Emission* emission = new ParticleModule::Emission();
	emission->SetRateOverTime(10.0f);
	m_particleSystem->SetEmission(emission);
	//m_particleSystem->SetShape()
}

//=============================================================
// [VehicleParticle] �I��
//=============================================================
void VehicleParticle::Uninit()
{
	m_particleSystem->Uninit();
	delete m_particleSystem;
}

//=============================================================
// [VehicleParticle] �X�V
//=============================================================
void VehicleParticle::Update()
{
	m_particleSystem->Update();
}

//=============================================================
// [VehicleParticle] �`��
//=============================================================
void VehicleParticle::Draw()
{
	m_particleSystem->Draw();
}