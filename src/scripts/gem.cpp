//=============================================================
//
// ジェム [gem.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "gem.h"
#include "shop/shop.h"
#include "component/3d/particle.h"

//=============================================================
// [Gem] 初期化
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
}

//=============================================================
// [Gem] 更新
//=============================================================
void Gem::Update()
{
	// 回転
	transform->Rotate(0.0f, 0.02f, 0.0f);
}

//=============================================================
// [Gem] トリガー
//=============================================================
void Gem::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Vehicle")
	{
		ShopManager::AddPoint(1);
		gameObject->Destroy();
	}
}