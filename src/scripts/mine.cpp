//=============================================================
//
// �n�� [mine.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "mine.h"
#include "vehicle.h"
#include "component/3d/particle.h"
#include "destructible.h"

//=============================================================
// [LandMine] ������
//=============================================================
void LandMine::Init()
{
	m_particle = new GameObject();
	auto particle = m_particle->AddComponent<ParticleSystem>();
	particle->SetShape(new ParticleShape::SphereShape());
	particle->GetEmission()->SetRateOverTime(50.0f);
	particle->Stop();
}

//=============================================================
// [LandMine] �X�V
//=============================================================
void LandMine::Update()
{

}

//=============================================================
// [LandMine] �����蔻��ɓ������Ƃ�
//=============================================================
void LandMine::OnTriggerEnter(GameObject* other)
{
	if (other->GetComponent<CVehicle>() != nullptr)
	{ // �o�C�N�ƐڐG�����Ƃ�
		// ������΂�
		auto dir = transform->GetWPos() - other->transform->GetWPos();
		D3DXVec3Normalize(&dir, &dir);
		CCollision::GetCollision(other)->GetRigidBody()->setLinearVelocity(btVector3(dir.x, -0.2f, dir.z) * -100.0f);

		// �_���[�W��^����
		other->GetComponent<CVehicle>()->AddDamage(DAMAGE);

		// �����j������
		gameObject->GetComponent<Destructible>()->ForceRemove();
	}
}