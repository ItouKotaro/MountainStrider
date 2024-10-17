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
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 10.0f, 30.0f));
	gameObject->AddComponent<CRigidBody>()->EnableAlwayActive();
	gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(700.0f);

	// ハンドル
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Translate(0.0f, 8.0f, -25.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// 前輪の生成
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, -30.0f, -45.0f);
	m_pFrontTire->AddComponent<CCylinderCollider>(15.0f, 15.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	CCollision::GetCollision(m_pFrontTire)->SetFriction(100);
	CCollision::GetCollision(m_pFrontTire)->SetMass(50);
	m_pFrontTire->AddComponent<CRigidBody>()->EnableAlwayActive();

	// 後輪の生成
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -30.0f, 23.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(15.0f, 15.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	CCollision::GetCollision(m_pBackTire)->SetFriction(100);
	CCollision::GetCollision(m_pBackTire)->SetMass(50);
	m_pBackTire->AddComponent<CRigidBody>()->EnableAlwayActive();

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
	pFrontHinge->enableMotor(0, true);
	pFrontHinge->setUpperLimit(D3DX_PI * 0.3f);
	pFrontHinge->setLowerLimit(-D3DX_PI * 0.3f);
	pFrontHinge->setLinearUpperLimit(btVector3(0.0f, 0.0f, 2.0f));
	pFrontHinge->setLinearLowerLimit(btVector3(0.0f, 0.0f, 0.0f));

	// 後輪の設定
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	pBackHinge->enableMotor(0, true);
	pBackHinge->setUpperLimit(D3DX_PI * 0.3f);
	pBackHinge->setLowerLimit(-D3DX_PI * 0.3f);
	pBackHinge->setLinearUpperLimit(btVector3(0.0f, 0.0f, 2.0f));
	pBackHinge->setLinearLowerLimit(btVector3(0.0f, 0.0f, 0.0f));

	// 前輪の回転設定
	auto pRotationalFront = pFrontHinge->getRotationalLimitMotor(0);
	pRotationalFront->m_enableMotor = true;
	pRotationalFront->m_hiLimit = btRadians(600);
	pRotationalFront->m_maxMotorForce = 500000;

	// 後輪の回転設定
	auto pRotationalBack = pBackHinge->getRotationalLimitMotor(0);
	pRotationalBack->m_enableMotor = true;
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
		auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2()->getRotationalLimitMotor(0);
		
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