//=============================================================
//
// �ԗ� [vehicle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle.h"
#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

//=============================================================
// [CVehicle] ������
//=============================================================
void CVehicle::Init()
{
	// �o�C�N�𐶐�����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 10.0f, 30.0f));
	gameObject->AddComponent<CRigidBody>()->EnableAlwayActive();
	gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(700.0f);

	// �n���h��
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Translate(0.0f, 8.0f, -25.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// �O�ւ̐���
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, -30.0f, -45.0f);
	m_pFrontTire->AddComponent<CCylinderCollider>(15.0f, 15.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	CCollision::GetCollision(m_pFrontTire)->SetFriction(100);
	CCollision::GetCollision(m_pFrontTire)->SetMass(50);
	m_pFrontTire->AddComponent<CRigidBody>()->EnableAlwayActive();

	// ��ւ̐���
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -30.0f, 23.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(15.0f, 15.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	CCollision::GetCollision(m_pBackTire)->SetFriction(100);
	CCollision::GetCollision(m_pBackTire)->SetMass(50);
	m_pBackTire->AddComponent<CRigidBody>()->EnableAlwayActive();

	// �q���W�̐ݒ�
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

	// �O�ւ̐ݒ�
	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	pFrontHinge->enableMotor(0, true);
	pFrontHinge->setTargetVelocity(0, btRadians(90));
	pFrontHinge->setMaxMotorForce(0, btRadians(500));
	pFrontHinge->setAngularUpperLimit(btVector3(D3DX_PI * 0.5f, 0.01f, 0.0f));
	pFrontHinge->setAngularLowerLimit(btVector3(-D3DX_PI * 0.5f, -0.01f, 0.0f));

	// ��ւ̐ݒ�
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	pBackHinge->enableMotor(0, true);
	pBackHinge->setTargetVelocity(0, btRadians(90));
	pBackHinge->setMaxMotorForce(0, btRadians(500));
	pBackHinge->setAngularUpperLimit(btVector3(D3DX_PI * 0.5f, 0.01f, 0.0f));
	pBackHinge->setAngularLowerLimit(btVector3(-D3DX_PI * 0.5f, -0.01f, 0.0f));

	// �O�ւ̉�]�ݒ�
	auto pRotationalFront = pFrontHinge->getRotationalLimitMotor(0);
	pRotationalFront->m_enableMotor = true;
	pRotationalFront->m_hiLimit = 10.0f;
	pRotationalFront->m_targetVelocity = 5000.0f;
	pRotationalFront->m_maxMotorForce = 100000.0f;
	pRotationalFront->m_bounce = 10.0f;
	
	// ��ւ̉�]�ݒ�
	auto pRotationalBack = pBackHinge->getRotationalLimitMotor(0);
	pRotationalBack->m_enableMotor = true;
	pRotationalBack->m_hiLimit = 10.0f;
	pRotationalBack->m_targetVelocity = 5000.0f;
	pRotationalBack->m_maxMotorForce = 100000.0f;
	pRotationalBack->m_bounce = 10.0f;
}

//=============================================================
// [CVehicle] �I��
//=============================================================
void CVehicle::Uninit()
{

}

//=============================================================
// [CVehicle] �X�V
//=============================================================
void CVehicle::Update()
{
	if (INPUT_INSTANCE->onPress("space"))
	{
		auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
		
		// ���[�^�[

	}
	//else
	//{
	//	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	//	// ���[�^�[
	//	btRotationalLimitMotor2* rotMotor = pFrontHinge->getRotationalLimitMotor(0);
	//	rotMotor->m_enableMotor = true;
	//	rotMotor->m_targetVelocity = btRadians(0.0);
	//}
}