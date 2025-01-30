//=============================================================
//
// 隕石イベント [meteo.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "meteo.h"
#include "manager.h"

#include "component/3d/particle.h"
#include "component/3d/collision.h"
#include "scripts/destructible.h"
#include "scripts/vehicle.h"

AudioClip MeteoCollision::m_fallSE = nullptr;
AudioClip MeteoCollision::m_landingSE = nullptr;

//=============================================================
// [MeteoEvent] 初期化
//=============================================================
void MeteoEvent::Init()
{
	// イベント時間を設定する
	m_eventTimer = EVENT_BASE_TIME + Benlib::RandomFloat(0.0f, EVENT_RANDOM_TIME);
}

//=============================================================
// [MeteoEvent] 終了
//=============================================================
void MeteoEvent::Uninit()
{

}

//=============================================================
// [MeteoEvent] 更新
//=============================================================
void MeteoEvent::Update()
{
	// タイマーを進める
	m_generateTimer -= CManager::GetInstance()->GetDeltaTime();
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();
	
	// 生成タイマーが終了したとき
	if (m_generateTimer <= 0.0f)
	{
		// 隕石を落とす
		Generate();

		// タイマーを再セット
		m_generateTimer = GENERATE_BASE_TIME + Benlib::RandomFloat(0.0f, GENERATE_RANDOM_TIME);
	}

	// イベントタイマーが終了したとき
	if (m_eventTimer <= 0.0f)
	{
		// イベントを終了する
		EndEvent();
	}
}

//=============================================================
// [MeteoEvent] 生成
//=============================================================
void MeteoEvent::Generate()
{
	// バイクを取得する
	GameObject* playerObj = GameObject::Find("Vehicle");

	// 位置を決める
	Transform trans;
	D3DXVECTOR3 pos = playerObj->transform->GetWPos();
	pos.x += Benlib::RandomFloat(-METEO_RANGE, METEO_RANGE);
	pos.y += 400.0f;
	pos.z += Benlib::RandomFloat(-METEO_RANGE, METEO_RANGE);
	trans.SetPos(pos);

	// 隕石を追加する
	GameObject* meteoObj = GameObject::LoadPrefab("data\\PREFAB\\props\\meteo00.pref", trans);
	meteoObj->AddComponent<MeteoCollision>();
}

class MeteoParticle : public ParticleModule::Shape
{
public:
	ResultData GetResult() override
	{
		ResultData data;
		float angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);
		float radius = Benlib::RandomFloat(0.0f, 10.0f);
		data.position = { sinf(angle) * radius, -10.0f, cosf(angle) * radius };
		data.direction = { sinf(angle), 1.0f, cosf(angle) };
		return data;
	}
};

//=============================================================
// [MeteoCollision] 初期化
//=============================================================
void MeteoCollision::Init()
{
	// パーティクルを作成する
	m_particle = new GameObject();
	m_particle->SetParent(gameObject);
	auto particle = m_particle->AddComponent<ParticleSystem>();
	particle->SetShape(new MeteoParticle());
	particle->GetEmission()->SetRateOverTime(30.0f);
	particle->SetGravity(-0.1f);
	particle->SetSize(50.0f);
	particle->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\smoke00.png");

	// 音の生成
	if (!m_fallSE) m_fallSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\EVENT\\meteo_fall.mp3", FMOD_3D, FMOD_LOOP_NORMAL);
	if (!m_landingSE) m_landingSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\EVENT\\meteo_landing.mp3", FMOD_3D);

	// 音再生オブジェクトの生成
	m_se = new GameObject();
	m_se->SetParent(gameObject);
	m_se->AddComponent<AudioSource>()->Play(m_fallSE);
	m_se->GetComponent<AudioSource>()->GetChannel()->set3DMinMaxDistance(1000.0f, 10.0f);
	m_se->GetComponent<AudioSource>()->GetChannel()->setVolume(100.0f);
}

//=============================================================
// [MeteoCollision] 地面とヒットしたとき
//=============================================================
void MeteoCollision::Update()
{
	btVector3 Start = btVector3(transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z);
	btVector3 End = Start + btVector3(0.0f, -300.0f, 0.0f);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{
		float y = Start.getY() - RayCallback.m_hitPointWorld.getY();

		if (y < 10.0f)
		{
			HitAction();
			return;
		}
	}
}

//=============================================================
// [MeteoCollision] 動作
//=============================================================
void MeteoCollision::HitAction()
{
	// リジッドボディを外す
	gameObject->Destroy(gameObject->GetComponent<CRigidBody>());
	CCollision::GetCollision(gameObject)->UpdateCollision();

	// 爆発音を生成する
	m_se->GetComponent<AudioSource>()->Play(m_landingSE);
	m_se->GetComponent<AudioSource>()->GetChannel()->setVolume(100.0f);
	m_se->GetComponent<AudioSource>()->GetChannel()->set3DMinMaxDistance(2000.0f, 10.0f);
	m_se->GetComponent<AudioSource>()->IsEndDestroy();

	// パーティクルを止める
	m_particle->GetComponent<ParticleSystem>()->Stop();

	// 周囲のオブジェクトを吹き飛ばす
	std::vector<GameObject*> allObjects = GameObject::GetAllGameObjects();
	for (auto itr = allObjects.begin(); itr != allObjects.end(); itr++)
	{
		if (Benlib::PosPlaneDistance((*itr)->transform->GetWPos(), transform->GetWPos()) < RANGE)
		{
			// オブジェクト
			if ((*itr)->GetComponent<Destructible>() != nullptr)
			{
				(*itr)->GetComponent<Destructible>()->ForceHit();

				// 吹き飛ばす
				auto dir = (*itr)->transform->GetWPos() - transform->GetWPos();
				D3DXVec3Normalize(&dir, &dir);
				CCollision::GetCollision(*itr)->GetRigidBody()->applyCentralImpulse(btVector3(dir.x, 1.0f, dir.z) * 1000.0f);
			}

			// バイク
			if ((*itr)->GetComponent<CVehicle>() != nullptr)
			{
				// 吹き飛ばす
				auto dir = (*itr)->transform->GetWPos() - transform->GetWPos();
				D3DXVec3Normalize(&dir, &dir);
				CCollision::GetCollision(*itr)->GetRigidBody()->setLinearVelocity(btVector3(dir.x, 1.0f, dir.z) * 100.0f);

				// 耐久値を減らす
				(*itr)->GetComponent<CVehicle>()->AddDamage(VEHICLE_DAMAGE);
			}
		}
	}

	gameObject->Destroy(this);
}