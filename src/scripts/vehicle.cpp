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
const float CVehicle::MAX_ENGINEFORCE = 300000.0f;
const float CVehicle::MAX_STEERING = 50000.0f;

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
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 10.0f, 20.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	CCollision::GetCollision(gameObject)->SetMass(250.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();
	gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, -120.0f, 0.0f));

	GameObject* pBodyModel = new GameObject;
	pBodyModel->SetParent(gameObject);
	pBodyModel->transform->Translate(0.0f, 25.0f, 0.0f);
	pBodyModel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");

	// バイクの軸固定
	//gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(1.0f, 1.0f, 0.0f));

	// ハンドル
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Rotate(0.0f, D3DX_PI, 0.0f);
	m_pHandle->transform->Translate(0.0f, 29.0f, 10.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// 前輪の生成
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, -5.0f, -25.0f);
	m_pFrontTire->AddComponent<CCylinderCollider>(10.0f, 3.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pFrontTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pFrontTire)->SetMass(60.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(900);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// 後輪の生成
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -5.0f, 20.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(10.0f, 3.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackTire)->SetMass(40.0f);
	CCollision::GetCollision(m_pBackTire)->SetFriction(900);
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

	pFrontHinge->enableSpring(2, true);
	pBackHinge->enableSpring(2, true);
	pFrontHinge->setStiffness(2, 5.0f);
	pBackHinge->setStiffness(2, 5.0f);
	pFrontHinge->setDamping(2, 1.0f);
	pBackHinge->setDamping(2, 1.0f);

	// パラメーター設定
	//pFrontHinge->setParam(BT_CONSTRAINT_CFM, -1.0f, 2);
	//pFrontHinge->setParam(BT_CONSTRAINT_ERP, 0.35f, 2);
	//pFrontHinge->setDamping(2, 1.0);
	//pFrontHinge->setStiffness(2, 80.0);

	pFrontHinge->setUpperLimit(D3DX_PI * 0.1f);
	pFrontHinge->setLowerLimit(-D3DX_PI * 0.1f);
	pBackHinge->setUpperLimit(0.0f);
	pBackHinge->setLowerLimit(0.0f);


	m_pSpeedText = new GameObject;
	m_pSpeedText->AddComponent<CText>();
	m_pSpeedText->GetComponent<CText>()->SetFontSize(50.0f);
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
	float ang = transform->GetWRotZ();
	CCollision::GetCollision(gameObject)->GetRigidBody()->setAngularVelocity(btVector3(sinf(transform->GetWRotY()) * -ang * 0.6f, 0.0f, cosf(transform->GetWRotY()) * -ang * 0.6f));

	CCollision::GetCollision(gameObject)->GetRigidBody()->applyCentralForce(btVector3(0.0f, -20000.0f, 0.0f));

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
		pBodyRB->setAngularVelocity(btVector3(sinf(transform->GetRotY()) * 0.8f, 0.0f, cosf(transform->GetRotY()) * 0.8f));
		m_fSteering += STEERING_VALUE;
	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		pBodyRB->setAngularVelocity(btVector3(sinf(transform->GetRotY()) * -0.8f, 0.0f, cosf(transform->GetRotY()) * -0.8f));
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
}
