//=============================================================
//
// 地雷 [mine.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "mine.h"
#include "vehicle.h"
#include "component/3d/particle.h"
#include "destructible.h"

//=============================================================
// [LandMine] 初期化
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
// [LandMine] 更新
//=============================================================
void LandMine::Update()
{

}

//=============================================================
// [LandMine] 当たり判定に入ったとき
//=============================================================
void LandMine::OnTriggerEnter(GameObject* other)
{
	if (other->GetComponent<CVehicle>() != nullptr)
	{ // バイクと接触したとき
		// 吹き飛ばす
		auto dir = transform->GetWPos() - other->transform->GetWPos();
		D3DXVec3Normalize(&dir, &dir);
		CCollision::GetCollision(other)->GetRigidBody()->setLinearVelocity(btVector3(dir.x, -0.2f, dir.z) * -100.0f);

		// ダメージを与える
		other->GetComponent<CVehicle>()->AddDamage(DAMAGE);

		// 自らを破棄する
		gameObject->GetComponent<Destructible>()->ForceRemove();
	}
}