//=============================================================
//
// 車両パーティクル [vehicle_particle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_particle.h"
#include "scripts/trajectory.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/wreckage.h"
#include "manager.h"
#include "scene/game.h"

//=============================================================
// [VehicleParticle] 初期化
//=============================================================
void VehicleParticle::Init()
{
	// バイクを取得する
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// 地形を取得する
	m_terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();

	// 池を取得する
	m_lake = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetLake();


	// 軌跡の追加
	m_trajectory = new GameObject();
	m_trajectory->AddComponent<CTrajectory>()->SetShow(true);

	// 後方パーティクルの初期化
	m_backParticleTimer = 0.0f;
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
	// 池を取得する
	if (m_lake == nullptr)
		m_lake = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetLake();

	// 軌跡の更新
	UpdateTrajectory();

	// 後方パーティクルの更新
	UpdateBackParticle();
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

//=============================================================
// [VehicleParticle] 後方パーティクルの更新
//=============================================================
void VehicleParticle::UpdateBackParticle()
{
	if (!m_vehicle || !m_lake) return;

	// クールタイムが終わっていないとき
	if (m_backParticleTimer > 0.0f)
	{
		m_backParticleTimer -= CManager::GetInstance()->GetDeltaTime();
		return;
	}

	// 地面についていないとき
	if (m_vehicle->GetGroundDistance() > CVehicle::GROUND_DISTANCE)
		return;

	// 池の中のとき
	if (m_lake->IsEnabled() && m_vehicle->transform->GetWPos().y < m_lake->GetHeight() - BG_LAKE_SPACE)
		return;

	// 速さによってクールダウンタイムを変更する
	float speed = m_vehicle->GetSpeed();

	if (speed < BP_MIN_GENERATE) return;
	else if (speed < 25.0f) m_backParticleTimer = 0.4f;
	else if (speed < 35.0f) m_backParticleTimer = 0.3f;
	else if (speed < 50.0f) m_backParticleTimer = 0.2f;
	else if (speed < 60.0f) m_backParticleTimer = 0.15f;
	else if (speed < 80.0f) m_backParticleTimer = 0.1f;
	else m_backParticleTimer = 0.05f;

	// ランダム要素
	m_backParticleTimer += Benlib::RandomFloat(-BP_RANDOM_TIME, BP_RANDOM_TIME);

	// ボックスを生成する
	GameObject* boxObj = new GameObject("VehicleParticle", "Particle");

	// 生成位置を決める
	D3DXVECTOR3 generatePos = { Benlib::RandomFloat(-BP_RANDOM_WIDTH, BP_RANDOM_WIDTH), 0.0f, BP_BACK_RANGE };
	D3DXMATRIX mtx = transform->GetMatrix();
	D3DXVec3TransformCoord(&generatePos, &generatePos, &mtx);
	boxObj->transform->SetPos(generatePos);

	// サイズを決める
	float size = Benlib::RandomFloat(BP_MIN_SIZE, BP_MIN_SIZE + speed * BP_SPEED_RATE);
	boxObj->transform->SetScale(size);

	// 回転を決める
	boxObj->transform->SetRot(Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f), Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f), 0.0f);

	// 現在位置から色を決める
	auto heightColor = m_terrain->GetHeightColor(transform->GetWPos().y);

	// 池の場合は色を変更する
	if (m_lake->IsEnabled() && m_vehicle->transform->GetWPos().y < m_lake->GetHeight())
	{
		heightColor = m_lake->GetBaseColor();
	}

	// 実体を生成する
	boxObj->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\vehicle_box.x");
	boxObj->GetComponent<CMesh>()->SetColor(heightColor);
	boxObj->GetComponent<CMesh>()->SetCustomColor(true);
	boxObj->AddComponent<CBoxCollider>(D3DXVECTOR3(size, size, size) * 2.0f);
	boxObj->AddComponent<CRigidBody>()->GetRigidBody()->setLinearVelocity(
		btVector3(sinf(transform->GetRot().y +D3DX_PI *0.5f) * Benlib::RandomFloat(-BP_EXPANSE_VALUE, BP_EXPANSE_VALUE),
			Benlib::RandomFloat(30.0f, 30.0f + speed * 0.2f),
			cosf(transform->GetRot().y + D3DX_PI * 0.5f) * Benlib::RandomFloat(-BP_EXPANSE_VALUE, BP_EXPANSE_VALUE))
	);
	boxObj->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, BP_GRAVITY, 0.0f));
	boxObj->AddComponent<CWreckage>();
}