//=============================================================
//
// 車両 [vehicle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle.h"
#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

//=============================================================
// [CVehicle] 初期化
//=============================================================
void CVehicle::Init()
{
	// バイクを生成する
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 5.0f, 20.0f));
	gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(100.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ハンドル
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Translate(0.0f, 8.0f, -25.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// 前輪の生成
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, -30.0f, -45.0f);
	m_pFrontTire->AddComponent<CCylinderCollider>(16.0f, 4.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pFrontTire->AddComponent<CRigidBody>()->EnableAlwayActive();
	CCollision::GetCollision(m_pFrontTire)->SetMass(5.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(1110);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// 後輪の生成
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -30.0f, 23.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(16.0f, 4.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackTire)->SetMass(5.0f);
	CCollision::GetCollision(m_pBackTire)->SetFriction(1110);
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
	pBackHinge->setMaxMotorForce(3, 30000);
	pBackHinge->setTargetVelocity(3, 0);

	// ステアリングエンジン
	pBackHinge->enableMotor(5, true);
	pBackHinge->setMaxMotorForce(5, 2200);
	pBackHinge->setTargetVelocity(5, 0);

	// パラメーター設定
	pBackHinge->setParam(BT_CONSTRAINT_CFM, 0.15f, 2);
	pBackHinge->setParam(BT_CONSTRAINT_ERP, 0.35f, 2);
	pBackHinge->setDamping(2, 1.0);
	pBackHinge->setStiffness(2, 40.0);

	pFrontHinge->setUpperLimit(0.0f);
	pFrontHinge->setLowerLimit(0.0f);
	pBackHinge->setUpperLimit(0.0f);
	pBackHinge->setLowerLimit(0.0f);

	// 車体の設定
	btRigidBody* pBodyRB = CCollision::GetCollision(gameObject)->GetRigidBody();
	//pBodyRB->setCenterOfMassTransform(btTransform::getIdentity());
	//pBodyRB->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	pBodyRB->setAngularFactor(btVector3(1.0f, 1.0f, 0.0f));


	//// 前輪の回転設定
	//auto pRotationalFront = pFrontHinge->getRotationalLimitMotor(0);
	//pRotationalFront->m_enableMotor = true;
	//pRotationalFront->m_hiLimit = btRadians(600);
	//pRotationalFront->m_maxMotorForce = 500000;

	//// 後輪の回転設定
	//auto pRotationalBack = pBackHinge->getRotationalLimitMotor(0);
	//pRotationalBack->m_enableMotor = true;
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
	if (INPUT_INSTANCE->onPress("space"))
	{
		auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
		auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();
		
		//pFrontHinge->setTargetVelocity(3, 200.0f);
		pBackHinge->setTargetVelocity(3, 4000.0f);
		// モーター
		//pFrontHinge->m_targetVelocity = btRadians(120);
	}
	//else
	//{
	//	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	//	// モーター
	//	btRotationalLimitMotor2* rotMotor = pFrontHinge->getRotationalLimitMotor(0);
	//	rotMotor->m_enableMotor = true;
	//	rotMotor->m_targetVelocity = btRadians(0.0);
	//}
}