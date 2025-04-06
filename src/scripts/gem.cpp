//=============================================================
//
// ジェム [gem.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "gem.h"
#include "shop/shop.h"
#include "component/3d/particle.h"
#include "component/3d/mesh.h"

AudioClip Gem::m_pickupSE = nullptr;

//=============================================================
// [Gem] 初期化
//=============================================================
void Gem::Init()
{
	// 音の生成
	if (!m_pickupSE) m_pickupSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\HIT\\gem.mp3");

	// パーティクルの作成
	m_particle = new GameObject();
	m_particle->transform->SetPos(0.0f, -15.0f, 0.0f);
	m_particle->SetParent(gameObject);
	m_particle->AddComponent<ParticleSystem>();
	m_particle->GetComponent<ParticleSystem>()->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\particle01.png");
	m_particle->GetComponent<ParticleSystem>()->SetGravity(-0.005f);
	ParticleModule::Emission* emission = new ParticleModule::Emission();
	emission->SetRateOverTime(1.5f);
	m_particle->GetComponent<ParticleSystem>()->SetEmission(emission);

	// カメラを取得する
	m_camera = GameObject::Find("Camera");
}

//=============================================================
// [Gem] 更新
//=============================================================
void Gem::Update()
{
	// 回転
	transform->Rotate(0.0f, 0.02f, 0.0f);

	// カメラの位置によって表示するかを決める
	gameObject->FindNameChildren("model")->GetComponent<CMesh>()->enabled
		= Benlib::PosPlaneDistance(transform->GetWPos(), m_camera->transform->GetWPos()) <= RENDER_DISTANCE;
}

//=============================================================
// [Gem] トリガー
//=============================================================
void Gem::OnTriggerEnter(GameObject* other)
{
	if (other->GetName() == "Vehicle")
	{
		ShopManager::AddPoint(POINT_VALUE);

		// 音を鳴らす
		GameObject* sePlayer = new GameObject();
		sePlayer->AddComponent<AudioSource>()->Play(m_pickupSE);
		sePlayer->GetComponent<AudioSource>()->IsEndDestroy();

		// 破棄する
		gameObject->Destroy();
	}
}