//=============================================================
//
// 車両 [vehicle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle.h"
#include "manager.h"

#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "component/2d/text.h"
#include "component/3d/motion.h"

#include "scripts/status_ui.h"
#include "scripts/vehicle_action.h"
#include "scripts/vehicle_sound.h"
#include "scripts/vehicle_particle.h"
#include "scene/game.h"

const float CVehicle::ENGINEFORCE_VALUE = 45.0f;
const float CVehicle::STEERING_VALUE = 3.0f;
const float CVehicle::MIN_ENGINEFORCE_VALUE = 1.0f;
const float CVehicle::MAX_ENGINEFORCE = 150000.0f;
const float CVehicle::MAX_STEERING = 15000.0f;
const float CVehicle::MAX_FUEL = 3000.0f;
const float CVehicle::MAX_ENDURANCE = 300.0f;
const float CVehicle::FLYING_DISTANCE = 30.0f;
const float CVehicle::GROUND_DISTANCE = 5.0f;
const int CVehicle::VIBRATION_VALUE = 10000;
const float CVehicle::GRAVITY = -80.0f;

float CVehicle::m_fuel = CVehicle::MAX_FUEL;
float CVehicle::m_endurance = CVehicle::MAX_ENDURANCE;

//=============================================================
// [CVehicle] 初期化
//=============================================================
void CVehicle::Init()
{
	// 変数の初期化
	m_measureCounter = 0;
	m_measurePos = transform->GetWPos();
	m_pStatusUI = nullptr;
	m_fuelConsumption = 0.0f;
	m_limitField = CCollision::GetCollision(GameObject::Find("LimitField"));

	// プレイヤーを生成する
	m_pPlayer = GameObject::LoadPrefab("data\\PREFAB\\player.pref");
	m_pPlayer->SetParent(gameObject);
	m_pPlayer->transform->SetScale(0.7f);
	m_pPlayer->GetComponent<CMotionManager>()->Play("ride");

	// バイクを生成する
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(1.8f, 1.8f, 6.0f), D3DXVECTOR3(0.0f, 3.0f, 0.0f));
	CCollision::GetCollision(gameObject)->SetMass(300.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();
	gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, GRAVITY, 0.0f));
	gameObject->AddComponent<VehicleAction>();
	gameObject->AddComponent<VehicleSound>();
	gameObject->AddComponent<VehicleParticle>();

	// 車体
	GameObject* pBodyModel = new GameObject;
	pBodyModel->SetParent(gameObject);
	pBodyModel->transform->Translate(0.0f, 5.5f, 0.0f);
	pBodyModel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");

	// ハンドル
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Translate(0.0f, 11.0f, -8.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// 前輪の生成
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, 0.0f, -12.5f);
	m_pFrontTire->AddComponent<CCylinderCollider>(3.5f, 1.05f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\front.x");
	m_pFrontTire->AddComponent<CRigidBody>();
	m_pFrontTire->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, -54.0f, 0.0f));
	CCollision::GetCollision(m_pFrontTire)->SetMass(9.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(100);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// 後輪の生成
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, 0.0f, 6.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(3.5f, 1.05f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\back.x");
	m_pBackTire->AddComponent<CRigidBody>();
	m_pBackTire->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, -54.0f, 0.0f));
	CCollision::GetCollision(m_pBackTire)->SetMass(9.0f);
	CCollision::GetCollision(m_pBackTire)->SetFriction(100);
	m_pBackTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ヒンジの設定
	m_pFrontTire->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(gameObject)->GetRigidBody(),
		CCollision::GetCollision(m_pFrontTire)->GetRigidBody(),
		m_pFrontTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);
	m_pBackTire->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(gameObject)->GetRigidBody(),
		CCollision::GetCollision(m_pBackTire)->GetRigidBody(),
		m_pBackTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);

	// 前輪の設定
	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	// ドライブエンジン
	pBackHinge->enableMotor(3, true);
	pBackHinge->setMaxMotorForce(3, MAX_ENGINEFORCE);
	pBackHinge->setTargetVelocity(3, 0);

	// ステアリングエンジン
	pFrontHinge->enableMotor(5, true);
	pFrontHinge->setMaxMotorForce(5, MAX_STEERING);
	pFrontHinge->setTargetVelocity(5, 0);

	// その他の設定
	pFrontHinge->enableSpring(2, true);
	pBackHinge->enableSpring(2, true);
	pFrontHinge->setStiffness(2, 1.0f);
	pBackHinge->setStiffness(2, 1.0f);
	pFrontHinge->setDamping(2, 0.00f);
	pBackHinge->setDamping(2, 0.00f);

	// 回転制限
	pFrontHinge->setUpperLimit(D3DX_PI* 0.06f);
	pFrontHinge->setLowerLimit(D3DX_PI * -0.06f);
	pBackHinge->setUpperLimit(0.0f);
	pBackHinge->setLowerLimit(0.0f);

	// ポーズを取得する
	m_pause = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetPause();
}

//=============================================================
// [CVehicle] 終了
//=============================================================
void CVehicle::Uninit()
{
}

//=============================================================
// [CVehicle] 更新
//=============================================================
void CVehicle::Update()
{
	if (m_pause->GetPause()) return;

	// 操作処理
	m_flying ? FlyingControlVehicle() : LandingControlVehicle();

	// 速度を計算する
	UpdateSpeedMeter();

	// ステータスUIの更新
	UpdateStatusUI();

	// 地面との距離を更新する
	UpdateGroundDistance();

	// ゲームオーバー処理
	if (m_fuel <= 0.0f)
	{ // 燃料が無くなったとき
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetCurrentScene()->pScene)->onGameOver();
	}
}

//=============================================================
// [CVehicle] バイクの操作
//=============================================================
void CVehicle::SetPos(const D3DXVECTOR3& pos)
{
	gameObject->transform->SetPos(pos);
	m_pFrontTire->transform->SetPos(pos);
	m_pBackTire->transform->SetPos(pos);
}

//=============================================================
// [CVehicle] ダメージを与える
//=============================================================
void CVehicle::AddDamage(const float& value)
{
	if (!gameObject->GetActive())
		return;

	// 耐久値を減らす
	m_endurance -= value;

	// 耐久値が無くなったときの処理
	if (m_endurance <= 0)
	{
		// ゲームオーバー処理
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetCurrentScene()->pScene)->onGameOver();
	}
}

//=============================================================
// [CVehicle] 燃料を増やす
//=============================================================
void CVehicle::AddFuel(const float& value)
{
	m_fuel = m_fuel + value > MAX_FUEL ? MAX_FUEL : m_fuel + value;
}

//=============================================================
// [CVehicle] 耐久値を増やす
//=============================================================
void CVehicle::AddEndurance(const float& value)
{
	m_endurance = m_endurance + value > MAX_ENDURANCE ? MAX_ENDURANCE : m_endurance + value;
}

//=============================================================
// [CVehicle] バイクの操作（地面）
//=============================================================
void CVehicle::LandingControlVehicle()
{
	// コントローラーの情報を取得する
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickLX = pGamepadDev->GetState().Gamepad.sThumbLX;
	short stickLY = pGamepadDev->GetState().Gamepad.sThumbLY;

	// 起き上がる方向にトルクを加える
	D3DXVECTOR3 angularVelocity;
	float ang = transform->GetWRotZ();
	angularVelocity = {
		sinf(transform->GetWRotY()) * -ang * 5.0f,
		0.0f,
		cosf(transform->GetWRotY()) * -ang * 5.0f
	};

	// ジャンプアクション
	if (INPUT_INSTANCE->onInput("jump") && m_groundDistance <= GROUND_DISTANCE)
	{
		CCollision::GetCollision(gameObject)->GetRigidBody()->setLinearVelocity(CCollision::GetCollision(gameObject)->GetRigidBody()->getLinearVelocity() + btVector3(0.0f, 50.0f, 0.0f));
	}

	// アクセル
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	if (m_fuel > 0.0f)
	{ // 燃料があるとき
		// アクセルを踏んでいるか
		bool isAccel = (INPUT_INSTANCE->onInput("accel") ||
			pGamepadDev->GetState().Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD ||
			pGamepadDev->GetState().Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD);

		// エンジン力を取得する
		float fEngineForce = isAccel ? ENGINEFORCE_VALUE : MIN_ENGINEFORCE_VALUE;

		// モーターを有効にする
		pBackHinge->enableMotor(3, isAccel);

		// タイヤを回転させる
		pBackHinge->setTargetVelocity(3, fEngineForce);

		// バイブレーション
		if (isAccel && INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER)
			INPUT_INSTANCE->GetInstance()->GetInputDevice<CGamepadDevice>()->SetVibration(VIBRATION_VALUE, VIBRATION_VALUE, 0.2f);

		// 燃料を減らす
		m_fuel -= fEngineForce * 0.01f;
		m_fuelConsumption += fEngineForce * 0.01f;
	}
	else
	{ // 燃料がないとき
		// タイヤの回転を止める
		pBackHinge->setTargetVelocity(3, 0.0f);
	}
	
	// 方向転換
	float fSteeringVelocity = 0.0f;
	if (INPUT_INSTANCE->onInput("left") || stickLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		angularVelocity += {sinf(transform->GetRotY()) * -3.0f, 0.0f, cosf(transform->GetRotY()) * -3.0f};
		fSteeringVelocity += STEERING_VALUE;

	}
	if (INPUT_INSTANCE->onInput("right") || stickLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		angularVelocity += {sinf(transform->GetRotY()) * 3.0f, 0.0f, cosf(transform->GetRotY()) * 3.0f};
		fSteeringVelocity -= STEERING_VALUE;
	}
	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	pFrontHinge->setTargetVelocity(5, fSteeringVelocity);

	// 傾き速度を適用する
	CCollision::GetCollision(gameObject)->GetRigidBody()->setAngularVelocity(btVector3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
}

//=============================================================
// [CVehicle] バイクの操作（空中）
//=============================================================
void CVehicle::FlyingControlVehicle()
{
	// 起き上がる方向にトルクを加える
	D3DXVECTOR3 angularVelocity;
	float ang = transform->GetWRotZ();
	angularVelocity = {
		sinf(transform->GetWRotY()) * -ang * 5.0f,
		0.0f,
		cosf(transform->GetWRotY()) * -ang * 5.0f
	};

	// コントローラーの情報を取得する
	auto pGamepadDev = INPUT_INSTANCE->GetInputDevice<CGamepadDevice>();
	short stickLX = pGamepadDev->GetState().Gamepad.sThumbLX;
	short stickLY = pGamepadDev->GetState().Gamepad.sThumbLY;

	// 回転
	if (INPUT_INSTANCE->onInput("left") || stickLX < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		angularVelocity += {0.0f, -5.5f, 0.0f};

	}
	if (INPUT_INSTANCE->onInput("right") || stickLX > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		angularVelocity += {0.0f, 5.5f, 0.0f};
	}

	// 傾き速度を適用する
	CCollision::GetCollision(gameObject)->GetRigidBody()->setAngularVelocity(btVector3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
}

//=============================================================
// [CVehicle] 速度計測
//=============================================================
void CVehicle::UpdateSpeedMeter()
{
	// 60フレーム毎でKm/hに換算
	m_measureCounter++;
	if (m_measureCounter >= 60)
	{
		float fDistance = Benlib::PosDistance(transform->GetWPos(), m_measurePos);
		m_fSpeed = fDistance / 2.0f;
		m_measureCounter = 0;
		m_measurePos = transform->GetWPos();
	}
}

//=============================================================
// [CVehicle] ステータスUIの更新
//=============================================================
void CVehicle::UpdateStatusUI()
{
	if (m_pStatusUI != nullptr)
	{
		// 燃料情報を更新する
		float fuelPercent = m_fuel / MAX_FUEL;
		m_pStatusUI->SetFuel(fuelPercent);

		// 耐久値情報を更新する
		float endurancePercent = m_endurance / MAX_ENDURANCE;
		m_pStatusUI->SetEndurance(endurancePercent);
	}
}

//=============================================================
// [CVehicle] 地面距離の更新
//=============================================================
void CVehicle::UpdateGroundDistance()
{
	// 地面との距離を計測する
	D3DXVECTOR3 frontPos = m_pFrontTire->transform->GetWPos();
	D3DXVECTOR3 backPos = m_pBackTire->transform->GetWPos();

	btVector3 Start;
	btVector3 End;
	float nearGroundDistance = 3000.0f;
	bool isHit = false;
	for (int i = 0; i < 2; i++)
	{
		Start = i == 0 ? btVector3(frontPos.x, frontPos.y, frontPos.z) : btVector3(backPos.x, backPos.y, backPos.z);
		End = Start + btVector3(0.0f, -6000.0f, 0.0f);

		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
		CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
		if (RayCallback.hasHit() && 
			RayCallback.m_collisionObject != CCollision::GetCollision(m_pFrontTire)->GetRigidBody() &&
			RayCallback.m_collisionObject != CCollision::GetCollision(m_pBackTire)->GetRigidBody() &&
			RayCallback.m_collisionObject != CCollision::GetCollision(gameObject)->GetRigidBody())
		{ // ヒットしたとき
			float distance = Start.getY() - RayCallback.m_hitPointWorld.getY();
			if (distance < nearGroundDistance)
			{
				nearGroundDistance = distance;
				isHit = true;
			}

			// もし地中に埋まっていた場合
			if (RayCallback.m_collisionObject == m_limitField->GetGhostObject())
			{
				if (fabsf(transform->GetWPos().x) < Terrain::TERRAIN_DISTANCE_HALF - Terrain::TERRAIN_SCALE &&
					fabsf(transform->GetWPos().z) < Terrain::TERRAIN_DISTANCE_HALF - Terrain::TERRAIN_SCALE)
				{
					SetPos({ transform->GetWPos().x, transform->GetWPos().y + 15.0f, transform->GetWPos().z });
				}
			}
		}
	}
	if (isHit)
	{
		m_groundDistance = nearGroundDistance;
	}

	// 飛んでいるか判定する
	if (!m_flying && m_groundDistance >= FLYING_DISTANCE)
	{ // 飛んでいると判定されたとき
		m_flying = true;
		m_startFlyingAngle = transform->GetRotY();
		m_flyingPosture = transform->GetRot();
		gameObject->GetComponent<VehicleAction>()->onFlyBegin();
	}

	// 着地したか判断する
	if (m_flying && m_groundDistance <= GROUND_DISTANCE)
	{ // 着地したと判断されたとき
		m_flying = false;
		gameObject->GetComponent<VehicleAction>()->onFlyEnd();
	}
}