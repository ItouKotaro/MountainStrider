//=============================================================
//
// 車両 [vehicle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle.h"
#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "component/2d/text.h"
#include "benlib.h"

const float CVehicle::ENGINEFORCE_VALUE = 50.0f;
const float CVehicle::STEERING_VALUE = 0.5f;

//=============================================================
// [CVehicle] 初期化
//=============================================================
void CVehicle::Init()
{
	// 変数の初期化
	m_fEngineForce = 0.0f;
	m_measureCounter = 0;
	m_measurePos = transform->GetWPos();

	// バイクを生成する
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 5.0f, 20.0f));
	gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(50.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();

	// バイクの軸固定
	gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));

	// ハンドル
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Rotate(0.0f, D3DX_PI, 0.0f);
	m_pHandle->transform->Translate(0.0f, 4.0f, 10.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// 前輪の生成
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, -18.0f, -30.0f);
	m_pFrontTire->AddComponent<CCylinderCollider>(10.0f, 2.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pFrontTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pFrontTire)->SetMass(10.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(100);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// 後輪の生成
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -18.0f, 23.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(10.0f, 2.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackTire)->SetMass(10.0f);
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
	pBackHinge->setMaxMotorForce(3, 25000);
	pBackHinge->setTargetVelocity(3, 0);

	// ステアリングエンジン
	pFrontHinge->enableMotor(5, true);
	pFrontHinge->setMaxMotorForce(5, 5000);
	pFrontHinge->setTargetVelocity(5, 0);

	// パラメーター設定
	pFrontHinge->setParam(BT_CONSTRAINT_CFM, 0.15f, 2);
	pFrontHinge->setParam(BT_CONSTRAINT_ERP, 0.35f, 2);
	pFrontHinge->setDamping(2, 1.0);
	pFrontHinge->setStiffness(2, 40.0);

	pFrontHinge->setUpperLimit(D3DX_PI * 0.07f);
	pFrontHinge->setLowerLimit(-D3DX_PI * 0.07f);
	pBackHinge->setUpperLimit(0.0f);
	pBackHinge->setLowerLimit(0.0f);


	m_pSpeedText = new GameObject;
	m_pSpeedText->AddComponent<CText>();
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
	// 起き上がる方向にトルクを加える
	//float ang = transform->GetRotZ();
	//CCollision::GetCollision(gameObject)->GetRigidBody()->applyTorque(btVector3(sinf(transform->GetWRotY()) * ang * ang * -2000.0f, 0.0f, cosf(transform->GetWRotY()) * ang * ang * -2000.0f));

	// 2軸ヒンジを取得する
	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	// 速度を計算する
	UpdateSpeedMeter();

	// アクセル
	if (INPUT_INSTANCE->onInput("accel"))
	{
		m_fEngineForce += ENGINEFORCE_VALUE;
	}
	m_fEngineForce += (0.0f - m_fEngineForce) * 0.002f;
	pBackHinge->setTargetVelocity(3, m_fEngineForce);

	// 軸の固定解除
	btRigidBody* pBodyRB = CCollision::GetCollision(gameObject)->GetRigidBody();
	if (INPUT_INSTANCE->onPress("p"))
	{
		pBodyRB->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
	}

	// 方向転換
	if (INPUT_INSTANCE->onPress("a"))
	{
		pBodyRB->applyTorqueImpulse(btVector3(sinf(transform->GetRotY()) * 500.0f, 0.0f, cosf(transform->GetRotY()) * 500.0f));
		m_fSteering += STEERING_VALUE;
	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		pBodyRB->applyTorqueImpulse(btVector3(sinf(transform->GetRotY()) * -500.0f, 0.0f, cosf(transform->GetRotY()) * -500.0f));
		m_fSteering -= STEERING_VALUE;
	}
	m_fSteering += (0.0f - m_fSteering) * 0.08f;

	pFrontHinge->setTargetVelocity(5, m_fSteering);
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

	// 状況を表示する
	m_pSpeedText->GetComponent<CText>()->SetText("速度: " + std::to_string(m_fSpeed) + "  |  エンジン: " + std::to_string(m_fEngineForce));
	m_pSpeedText->GetComponent<CText>()->SetFontSize(50.0f);
}




//=============================================================
// [CRaycastVehicle] 初期化
//=============================================================
void CRaycastVehicle::Init()
{
	// 物理ワールドを取得する
	auto& pDynamicsWorld = CPhysics::GetInstance()->GetDynamicsWorld();

	// バイクを生成する
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(2.0f, 10.0f, 20.0f), D3DXVECTOR3(0.0f, -9.0f, 0.0f));
	//gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(2.0f, 15.0f, 2.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f));
	gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(300.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();
	m_body = gameObject->GetComponent<CRigidBody>()->GetRigidBody();
	m_body->setGravity(btVector3(0.0f, -40.0f, 0.0f));

	// レイキャスター
	m_raycaster = new btDefaultVehicleRaycaster(&pDynamicsWorld);

	// 車両を作成する
	m_vehicle = new btRaycastVehicle(m_tuning, CCollision::GetCollision(gameObject)->GetRigidBody(), m_raycaster);
	m_vehicle->setCoordinateSystem(0, 1, 2);
	m_vehicle->resetSuspension();

	// 車輪を追加する
	m_vehicle->addWheel(		// 前輪
		{ 0.0f, -18.0f, 25.0f },
		{0.0f, -1.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		2.6f,
		10.0f,
		m_tuning,
		true
	);
	m_vehicle->addWheel(		// 後輪
		{ 0.0f, -18.0f, -25.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		2.6f,
		10.0f,
		m_tuning,
		false
	);

	// 車輪の設定
	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = 20.0f;
		wheel.m_wheelsDampingRelaxation = 2.3f;
		wheel.m_wheelsDampingCompression = 1.0f;
		wheel.m_frictionSlip = 1000.0f;
		wheel.m_rollInfluence = 0.1f;
	}

	// ワールドに追加する
	pDynamicsWorld.addAction(m_vehicle);


	// タイヤを生成する
	m_pFrontWheel = new GameObject;
	m_pFrontWheel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackWheel = new GameObject;
	m_pBackWheel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");

	m_pText = new GameObject;
	m_pText->AddComponent<CText>();
}

//=============================================================
// [CRaycastVehicle] 終了
//=============================================================
void CRaycastVehicle::Uninit()
{
	// 物理ワールドを取得する
	auto& pDynamicsWorld = CPhysics::GetDynamicsWorld();

	pDynamicsWorld.removeAction(m_vehicle);
	delete m_vehicle;
	m_vehicle = nullptr;

	delete m_raycaster;
	m_raycaster = nullptr;
}

//=============================================================
// [CRaycastVehicle] 更新
//=============================================================
void CRaycastVehicle::Update()
{
	// トルク
	float ang = m_body->getAngularVelocity().getZ();
	//m_body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	m_body->applyTorque(btVector3(ang * -1000000.0f * sinf(transform->GetRotY() - D3DX_PI), 0.0f, ang * -1000000.0f * cosf(transform->GetRotY() - D3DX_PI)));
	m_vehicle->applyEngineForce(4000.0f, 1);

	m_pText->GetComponent<CText>()->SetText(std::to_string(transform->GetRotY()));

	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		btTransform wheelTransform = m_vehicle->getWheelTransformWS(i);

		// タイヤ
		GameObject* pTargetWheel = i == 0 ? m_pFrontWheel : m_pBackWheel;
		pTargetWheel->transform->SetPos(wheelTransform.getOrigin().getX(), wheelTransform.getOrigin().getY(), wheelTransform.getOrigin().getZ());
		pTargetWheel->transform->SetQuaternion(D3DXQUATERNION(wheelTransform.getRotation().getX(), wheelTransform.getRotation().getY(), wheelTransform.getRotation().getZ(), wheelTransform.getRotation().getW()));
	}

	if (INPUT_INSTANCE->onPress("d"))
	{
		m_vehicle->setSteeringValue(0.3f, 0);
	}
	if (INPUT_INSTANCE->onPress("a"))
	{
		m_vehicle->setSteeringValue(-0.3f, 0);
	}
	if (INPUT_INSTANCE->onPress("w"))
	{
		m_vehicle->setSteeringValue(0.0f, 0);
	}

	// レイ
	UpdateRay();
}

void CRaycastVehicle::UpdateRay()
{
	btCollisionWorld::ClosestRayResultCallback RayCallback(
		btVector3(transform->GetPos().x, transform->GetPos().y + 2.0f, transform->GetPos().z),
		btVector3(transform->GetPos().x, transform->GetPos().y - 20.0f, transform->GetPos().z - 25.0f)
	);

	// Perform raycast
	CPhysics::GetDynamicsWorld().rayTest(btVector3(transform->GetPos().x, transform->GetPos().y + 2.0f, transform->GetPos().z),
		btVector3(transform->GetPos().x, transform->GetPos().y - 20.0f, transform->GetPos().z - 25.0f),
		RayCallback
	);
	if (RayCallback.hasHit())
	{
		m_body->setLinearVelocity(m_body->getLinearVelocity() + btVector3(0.0f, 1.0f, 0.0f));
	}
}