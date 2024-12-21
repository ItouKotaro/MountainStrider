//=============================================================
//
// 車両パーティクル [vehicle_particle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_particle.h"

//=============================================================
// [VehicleParticle] 初期化
//=============================================================
void VehicleParticle::Init()
{
	// バイクを取得する
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// パーティクルシステムを生成する
	m_particleSystem = new SingleComponent<ParticleSystem>();
	m_particleSystem->Init();
	//m_particleSystem->SetShape()
}

//=============================================================
// [VehicleParticle] 終了
//=============================================================
void VehicleParticle::Uninit()
{
	m_particleSystem->Uninit();
	delete m_particleSystem;
}

//=============================================================
// [VehicleParticle] 更新
//=============================================================
void VehicleParticle::Update()
{
	m_particleSystem->Update();
}

//=============================================================
// [VehicleParticle] 描画
//=============================================================
void VehicleParticle::Draw()
{
	m_particleSystem->Draw();
}