//=============================================================
//
// 車両パーティクル [vehicle_particle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_particle.h"
#include "scripts/trajectory.h"

//=============================================================
// [VehicleParticle] 初期化
//=============================================================
void VehicleParticle::Init()
{
	// バイクを取得する
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// 軌跡の追加
	m_trajectory = new GameObject();
	m_trajectory->AddComponent<CTrajectory>()->SetShow(true);
}

//=============================================================
// [VehicleParticle] 終了
//=============================================================
void VehicleParticle::Uninit()
{
}

//=============================================================
// [VehicleParticle] 更新
//=============================================================
void VehicleParticle::Update()
{
	// 軌跡の更新
	UpdateTrajectory();
}

//=============================================================
// [VehicleParticle] 描画
//=============================================================
void VehicleParticle::Draw()
{
}

//=============================================================
// [VehicleParticle] 軌跡の更新
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
