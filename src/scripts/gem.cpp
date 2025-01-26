//=============================================================
//
// �W�F�� [gem.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "gem.h"
#include "shop/shop.h"
#include "component/3d/particle.h"
#include "component/3d/mesh.h"

//=============================================================
// [Gem] ������
//=============================================================
void Gem::Init()
{
	m_particle = new GameObject();
	m_particle->transform->SetPos(0.0f, -15.0f, 0.0f);
	m_particle->SetParent(gameObject);
	m_particle->AddComponent<ParticleSystem>();
	m_particle->GetComponent<ParticleSystem>()->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\particle01.png");
	m_particle->GetComponent<ParticleSystem>()->SetGravity(-0.005f);
	ParticleModule::Emission* emission = new ParticleModule::Emission();
	emission->SetRateOverTime(1.5f);
	m_particle->GetComponent<ParticleSystem>()->SetEmission(emission);

	// �J�������擾����
	m_camera = GameObject::Find("Camera");
}

//=============================================================
// [Gem] �X�V
//=============================================================
void Gem::Update()
{
	// ��]
	transform->Rotate(0.0f, 0.02f, 0.0f);

	// �J�����̈ʒu�ɂ���ĕ\�����邩�����߂�
	gameObject->FindNameChildren("model")->GetComponent<CMesh>()->enabled
		= Benlib::PosPlaneDistance(transform->GetWPos(), m_camera->transform->GetWPos()) <= RENDER_DISTANCE;
}

//=============================================================
// [Gem] �g���K�[
//=============================================================
void Gem::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Vehicle")
	{
		ShopManager::AddPoint(5);
		gameObject->Destroy();
	}
}