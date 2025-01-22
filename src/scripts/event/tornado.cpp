//=============================================================
//
// 竜巻イベント [tornado.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "tornado.h"
#include "manager.h"
#include "component/3d/particle.h"
#include "scripts/destructible.h"
#include "scripts/vehicle.h"
#include "scene/game.h"

AudioClip TornadoEvent::m_tornadeSE = nullptr;

//=============================================================
// トルネード用パーティクル
//=============================================================
class TornadoParticle : public ParticleModule::Shape
{
public:
	ResultData GetResult()
	{
		ResultData data;
		
		// 位置
		float height = Benlib::RandomFloat(0.0f, TORNADO_HEIGHT);
		float radius = (height * height) / 100.0f;
		float angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);

		data.position.x = sinf(angle) * radius;
		data.position.y = height;
		data.position.z = cosf(angle) * radius;

		// 向き
		data.direction = { 0.0f, 1.0f, 0.0f };

		return data;
	}
private:
	const float TORNADO_HEIGHT = 100.0f;
};

//=============================================================
// [TornadoEvent] 初期化
//=============================================================
void TornadoEvent::Init()
{
	// 音の読み込み
	if (!m_tornadeSE) m_tornadeSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\EVENT\\tornade.mp3", FMOD_3D | FMOD_LOOP_NORMAL);
	
	// トルネードを作成する
	m_tornade = new GameObject();

	// 位置を決める
	D3DXVECTOR3 confPos;
	D3DXVECTOR3 vehiclePos = GameObject::Find("Vehicle")->transform->GetWPos();
	do
	{
		confPos = { Benlib::RandomFloat(-GENERATE_RADIUS, GENERATE_RADIUS), 0.0f, Benlib::RandomFloat(-GENERATE_RADIUS, GENERATE_RADIUS) };
		confPos += vehiclePos;
	} while (Benlib::PosPlaneDistance(confPos, vehiclePos) <= GENERATE_MIN_DISTANCE);
	m_tornade->transform->SetPos(confPos);


	// 音の設定
	m_tornade->AddComponent<AudioSource>()->Play(m_tornadeSE);
	m_tornade->GetComponent<AudioSource>()->GetChannel()->setVolume(80.0f);
	m_tornade->GetComponent<AudioSource>()->GetChannel()->set3DMinMaxDistance(800.0f, TORNADO_RANGE);

	// パーティクルの設定
	auto particle = m_tornade->AddComponent<ParticleSystem>();
	particle->SetShape(new TornadoParticle());
	particle->SetVortex(0.1f);
	particle->GetEmission()->SetRateOverTime(150.0f);
	particle->SetPower(0.5f, 1.0f);
	particle->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\smoke00.png");
	particle->SetSize(15.0f, 30.0f);
	particle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	// 地形の高度情報を取得する
	Terrain* terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();
	m_maxHeight = terrain->GetMaxHeight();
	m_minHeight = terrain->GetMinHeight();

	// イベント時間を設定する
	m_eventTimer = EVENT_BASE_TIME + Benlib::RandomFloat(0.0f, EVENT_RANDOM_TIME);
}

//=============================================================
// [TornadoEvent] 終了
//=============================================================
void TornadoEvent::Uninit()
{
	// トルネードを破棄する
	m_tornade->Destroy();
}

//=============================================================
// [TornadoEvent] 更新
//=============================================================
void TornadoEvent::Update()
{
	// 移動の更新
	UpdateMove();

	// アクションの更新
	UpdateAction();

	// タイマーを進める
	m_moveTimer -= CManager::GetInstance()->GetDeltaTime();
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();

	// 移動タイマーが終了したとき
	if (m_moveTimer <= 0.0f)
	{
		ResetMoveAngle();
	}

	// イベントタイマーが終了したとき
	if (m_eventTimer <= 0.0f)
	{
		// イベントを終了する
		EndEvent();
	}
}

//=============================================================
// [TornadoEvent] 向かう方向をリセットする
//=============================================================
void TornadoEvent::ResetMoveAngle()
{
	// 方向を決める
	m_moveAngle += Benlib::RandomFloat(-MOVE_ANGLE_RANGE, MOVE_ANGLE_RANGE);

	// 時間を決める
	m_moveTimer = MOVE_BASE_TIME + Benlib::RandomFloat(0.0f, MOVE_RANDOM_TIME);
}

//=============================================================
// [TornadoEvent] 移動の更新
//=============================================================
void TornadoEvent::UpdateMove()
{
	// 移動させる
	m_tornade->transform->Translate(sinf(m_moveAngle) * TORNADO_SPEED, 0.0f, cosf(m_moveAngle) * TORNADO_SPEED);

	// 高さを調節する
	btVector3 Start = btVector3(m_tornade->transform->GetWPos().x, m_maxHeight + 10.0f, m_tornade->transform->GetWPos().z);
	btVector3 End = Start + btVector3(0.0f, m_minHeight - 10.0f, 0.0f);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // 地面に接触したとき
		m_tornade->transform->SetPos(m_tornade->transform->GetPos().x, RayCallback.m_hitPointWorld.getY(), m_tornade->transform->GetPos().z);
	}
}

//=============================================================
// [TornadoEvent] アクションの更新
//=============================================================
void TornadoEvent::UpdateAction()
{
	// 周囲のオブジェクトを吹き飛ばす
	std::vector<GameObject*> allObjects = GameObject::GetAllGameObjects();
	for (auto itr = allObjects.begin(); itr != allObjects.end(); itr++)
	{
		if (Benlib::PosPlaneDistance((*itr)->transform->GetWPos(), m_tornade->transform->GetWPos()) < TORNADO_RANGE &&
			(*itr)->transform->GetWPos().y <= m_tornade->transform->GetWPos().y + TORNADO_HEIGHT)
		{
			// オブジェクト
			if ((*itr)->GetComponent<Destructible>() != nullptr)
			{
				(*itr)->GetComponent<Destructible>()->ForceHit(false);

				// トルネードに巻き込ませる
				D3DXVECTOR3 dir = (m_tornade->transform->GetWPos() - (*itr)->transform->GetWPos()) * 0.5f;
				dir.y = 3.0f;
				CCollision::GetCollision(*itr)->GetRigidBody()->applyCentralForce(btVector3(dir.x, dir.y, dir.z) * 350.0f);
			}

			// バイク
			if ((*itr)->GetComponent<CVehicle>() != nullptr)
			{
				(*itr)->GetComponent<CVehicle>()->AddDamage(0.3f);

				// トルネードに巻き込ませる
				D3DXVECTOR3 dir = (m_tornade->transform->GetWPos() - (*itr)->transform->GetWPos());
				dir.y = 5.0f;
				CCollision::GetCollision(*itr)->GetRigidBody()->applyCentralForce(btVector3(dir.x, dir.y, dir.z) * 8000.0f);
			}
		}
	}
}