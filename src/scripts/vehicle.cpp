//=============================================================
//
// �ԗ� [vehicle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle.h"
#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "component/2d/text.h"

//=============================================================
// [CVehicle] ������
//=============================================================
void CVehicle::Init()
{
	// �o�C�N�𐶐�����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 5.0f, 20.0f));
	gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(10.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();

	// �n���h��
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Translate(0.0f, 8.0f, -25.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// �O�ւ̐���
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, -30.0f, -45.0f);
	m_pFrontTire->AddComponent<CCylinderCollider>(16.0f, 5.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pFrontTire->AddComponent<CRigidBody>()->EnableAlwayActive();
	CCollision::GetCollision(m_pFrontTire)->SetMass(5.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(1110);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ��ւ̐���
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -30.0f, 23.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(16.0f, 5.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackTire)->SetMass(5.0f);
	CCollision::GetCollision(m_pBackTire)->SetFriction(1110);
	m_pBackTire->GetComponent<CRigidBody>()->EnableAlwayActive();

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
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	// �h���C�u�G���W��
	pBackHinge->enableMotor(3, true);
	pBackHinge->setMaxMotorForce(3, 20000);
	pBackHinge->setTargetVelocity(3, 0);

	// �X�e�A�����O�G���W��
	pBackHinge->enableMotor(5, true);
	pBackHinge->setMaxMotorForce(5, 30000);
	pBackHinge->setTargetVelocity(5, 0);

	// �p�����[�^�[�ݒ�
	pBackHinge->setParam(BT_CONSTRAINT_CFM, 0.15f, 2);
	pBackHinge->setParam(BT_CONSTRAINT_ERP, 0.35f, 2);
	pBackHinge->setDamping(2, 1.0);
	pBackHinge->setStiffness(2, 40.0);

	pFrontHinge->setUpperLimit(0.0f);
	pFrontHinge->setLowerLimit(0.0f);
	pBackHinge->setUpperLimit(0.0f);
	pBackHinge->setLowerLimit(0.0f);

	// �ԑ̂̐ݒ�
	btRigidBody* pBodyRB = CCollision::GetCollision(gameObject)->GetRigidBody();
	//pBodyRB->setCenterOfMassTransform(btTransform::getIdentity());
	//pBodyRB->setLinearVelocity(btVector3(0.0f, 0.0f, 0.0f));
	//pBodyRB->setAngularFactor(btVector3(1.0f, 1.0f, 0.0f));


	//// �O�ւ̉�]�ݒ�
	//auto pRotationalFront = pFrontHinge->getRotationalLimitMotor(0);
	//pRotationalFront->m_enableMotor = true;
	//pRotationalFront->m_hiLimit = btRadians(600);
	//pRotationalFront->m_maxMotorForce = 500000;

	//// ��ւ̉�]�ݒ�
	//auto pRotationalBack = pBackHinge->getRotationalLimitMotor(0);
	//pRotationalBack->m_enableMotor = true;
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
	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	if (INPUT_INSTANCE->onPress("space"))
	{
		pBackHinge->setTargetVelocity(3, 1000.0f);
	}

	if (INPUT_INSTANCE->onPress("a"))
	{
		btVector3 power = btVector3(200.0f, 0.0f, 0.0f);
		CCollision::GetCollision(gameObject)->GetRigidBody()->applyCentralForce(power);
	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		btVector3 power = btVector3(-200.0f, 0.0f, 0.0f);
		CCollision::GetCollision(gameObject)->GetRigidBody()->applyCentralForce(power);
	}
}






//=============================================================
// [CRaycastVehicle] ������
//=============================================================
void CRaycastVehicle::Init()
{
	// �������[���h���擾����
	auto& pDynamicsWorld = CPhysics::GetInstance()->GetDynamicsWorld();

	// �{�f�B�[�𐶐�����
	btCollisionShape* shapes = new btBoxShape(btVector3(5.0f, 10.0f, 20.0f));
	CPhysics::GetInstance()->GetCollisionShape().push_back(shapes);

	// �������[�����g
	btVector3 inertia(0, 0, 0);
	shapes->calculateLocalInertia(2.0f, inertia);

	// ���̑���
	m_motionstate = new btDefaultMotionState();

	// ���̍쐬
	btRigidBody::btRigidBodyConstructionInfo rb_info(2.0f, m_motionstate, shapes, inertia);
	m_body = new btRigidBody(rb_info);
	pDynamicsWorld.addRigidBody(m_body);
	//gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 10.0f, 20.0f));
	//gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	//gameObject->AddComponent<CRigidBody>();
	//gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();


	// ���C�L���X�^�[
	m_raycaster = new btDefaultVehicleRaycaster(&pDynamicsWorld);

	// �ԗ����쐬����
	m_vehicle = new btRaycastVehicle(m_tuning, m_body, m_raycaster);
	m_vehicle->setCoordinateSystem(0, 1, 2);

	// ���[���h�ɒǉ�����
	pDynamicsWorld.addVehicle(m_vehicle);

	// �ԗւ�ǉ�����
	m_vehicle->addWheel(		// �O��
		{0.0f, -5.0f, 20.0f},
		{0.0f, -1.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		0.6f,
		16.0f,
		m_tuning,
		true
	);
	m_vehicle->addWheel(		// ���
		{ 0.0f, -5.0f, -20.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		0.6f,
		16.0f,
		m_tuning,
		false
	);

	// �ԗւ̐ݒ�
	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = 20.0f;
		wheel.m_wheelsDampingRelaxation = 2.3f;
		wheel.m_wheelsDampingCompression = 4.4f;
		wheel.m_frictionSlip = 10.0f;
		wheel.m_rollInfluence = 0.01f;
	}

	m_pText = new GameObject;
	m_pText->AddComponent<CText>();
}

//=============================================================
// [CRaycastVehicle] �I��
//=============================================================
void CRaycastVehicle::Uninit()
{
	// �������[���h���擾����
	auto& pDynamicsWorld = CPhysics::GetDynamicsWorld();

	pDynamicsWorld.removeVehicle(m_vehicle);
	delete m_vehicle;
	m_vehicle = nullptr;

	delete m_raycaster;
	m_raycaster = nullptr;

	// ���W�b�h�{�f�B��j������
	pDynamicsWorld.removeRigidBody(m_body);
	delete m_body;
	m_body = nullptr;

	// ���[�V�����X�e�[�g��j������
	delete m_motionstate;
	m_motionstate = nullptr;
}

//=============================================================
// [CRaycastVehicle] �X�V
//=============================================================
void CRaycastVehicle::Update()
{
	m_vehicle->applyEngineForce(50.0f, 1);

	m_pText->GetComponent<CText>()->SetText(std::to_string(m_vehicle->getCurrentSpeedKmHour()));

	for (int i = 0; i < 2; ++i)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		btTransform wheelTransform = m_vehicle->getWheelTransformWS(i);
		btWheelInfo::RaycastInfo ray;
		wheel.updateWheel(*m_body, ray);
	}
}