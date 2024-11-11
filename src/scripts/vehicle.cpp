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
#include "benlib.h"

const float CVehicle::ENGINEFORCE_VALUE = 50.0f;
const float CVehicle::STEERING_VALUE = 0.5f;

//=============================================================
// [CVehicle] ������
//=============================================================
void CVehicle::Init()
{
	// �ϐ��̏�����
	m_fEngineForce = 0.0f;
	m_measureCounter = 0;
	m_measurePos = transform->GetWPos();

	// �o�C�N�𐶐�����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 10.0f, 20.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	CCollision::GetCollision(gameObject)->SetMass(400.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();
	gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, -120.0f, 0.0f));

	GameObject* pBodyModel = new GameObject;
	pBodyModel->SetParent(gameObject);
	pBodyModel->transform->Translate(0.0f, 25.0f, 0.0f);
	pBodyModel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");

	// �o�C�N�̎��Œ�
	//gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setAngularFactor(btVector3(1.0f, 1.0f, 0.0f));

	// �n���h��
	m_pHandle = new GameObject;
	m_pHandle->SetParent(gameObject);
	m_pHandle->transform->Rotate(0.0f, D3DX_PI, 0.0f);
	m_pHandle->transform->Translate(0.0f, 29.0f, 10.0f);
	m_pHandle->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\handle.x");

	// �O�ւ̐���
	m_pFrontTire = new GameObject;
	m_pFrontTire->transform->Translate(0.0f, -5.0f, -25.0f);
	m_pFrontTire->AddComponent<CCylinderCollider>(10.0f, 3.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pFrontTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pFrontTire)->SetMass(40.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(900);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ��ւ̐���
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -5.0f, 20.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(10.0f, 3.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackTire)->SetMass(40.0f);
	CCollision::GetCollision(m_pBackTire)->SetFriction(900);
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
	pBackHinge->setMaxMotorForce(3, 150000);
	pBackHinge->setTargetVelocity(3, 0);

	// �X�e�A�����O�G���W��
	pFrontHinge->enableMotor(5, true);
	pFrontHinge->setMaxMotorForce(5, 50000);
	pFrontHinge->setTargetVelocity(5, 0);

	pFrontHinge->enableSpring(2, true);
	pBackHinge->enableSpring(2, true);
	pFrontHinge->setStiffness(2, 5.0f);
	pBackHinge->setStiffness(2, 5.0f);
	pFrontHinge->setDamping(2, 1.0f);
	pBackHinge->setDamping(2, 1.0f);

	// �p�����[�^�[�ݒ�
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
	// �N���オ������Ƀg���N��������
	float ang = transform->GetWRotZ();
	CCollision::GetCollision(gameObject)->GetRigidBody()->setAngularVelocity(btVector3(sinf(transform->GetWRotY()) * -ang * 0.4f, 0.0f, cosf(transform->GetWRotY()) * -ang * 0.4f));
	//CCollision::GetCollision(m_pFrontTire)->GetRigidBody()->applyTorque(btVector3(sinf(transform->GetWRotY() + D3DX_PI) * ang * -100.0f, 0.0f, cosf(transform->GetWRotY() + D3DX_PI) * ang * -100.0f));
	//CCollision::GetCollision(m_pBackTire)->GetRigidBody()->applyTorque(btVector3(sinf(transform->GetWRotY() + D3DX_PI) * ang * -100.0f, 0.0f, cosf(transform->GetWRotY() + D3DX_PI) * ang * -100.0f));


	// 2���q���W���擾����
	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	// ���x���v�Z����
	UpdateSpeedMeter();

	// �A�N�Z��
	if (INPUT_INSTANCE->onInput("accel"))
	{
		m_fEngineForce += ENGINEFORCE_VALUE;
	}
	m_fEngineForce += (0.0f - m_fEngineForce) * 0.002f;
	pBackHinge->setTargetVelocity(3, m_fEngineForce);

	// ���̌Œ����
	btRigidBody* pBodyRB = CCollision::GetCollision(gameObject)->GetRigidBody();
	if (INPUT_INSTANCE->onPress("p"))
	{
		pBodyRB->setAngularFactor(btVector3(1.0f, 1.0f, 1.0f));
	}

	// �����]��
	if (INPUT_INSTANCE->onPress("a"))
	{
		pBodyRB->setAngularVelocity(btVector3(sinf(transform->GetRotY()) * 0.6f, 0.0f, cosf(transform->GetRotY()) * 0.6f));
		m_fSteering += STEERING_VALUE;
	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		pBodyRB->setAngularVelocity(btVector3(sinf(transform->GetRotY()) * -0.6f, 0.0f, cosf(transform->GetRotY()) * -0.6f));
		m_fSteering -= STEERING_VALUE;
	}
	m_fSteering += (0.0f - m_fSteering) * 0.08f;

	pFrontHinge->setTargetVelocity(5, m_fSteering);
}

//=============================================================
// [CVehicle] ���x�v��
//=============================================================
void CVehicle::UpdateSpeedMeter()
{
	// 60�t���[������Km/h�Ɋ��Z
	m_measureCounter++;
	if (m_measureCounter >= 60)
	{
		float fDistance = Benlib::PosDistance(transform->GetWPos(), m_measurePos);
		m_fSpeed = fDistance / 2.0f;
		m_measureCounter = 0;
		m_measurePos = transform->GetWPos();
	}

	// �󋵂�\������
	//m_pSpeedText->GetComponent<CText>()->SetText("���x: " + std::to_string(m_fSpeed) + "  |  �G���W��: " + std::to_string(m_fEngineForce));
	m_pSpeedText->GetComponent<CText>()->SetText(std::to_string(transform->GetWRot().z));
}




//=============================================================
// [CRaycastVehicle] ������
//=============================================================
void CRaycastVehicle::Init()
{
	// �������[���h���擾����
	auto& pDynamicsWorld = CPhysics::GetInstance()->GetDynamicsWorld();

	// �o�C�N�𐶐�����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(2.0f, 8.0f, 20.0f), D3DXVECTOR3(0.0f, -9.0f, 0.0f));
	//gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(2.0f, 15.0f, 2.0f), D3DXVECTOR3(0.0f, -10.0f, 0.0f));
	gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(300.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();
	m_body = gameObject->GetComponent<CRigidBody>()->GetRigidBody();
	//m_body->setGravity(btVector3(0.0f, -40.0f, 0.0f));

	// ���C�L���X�^�[
	m_raycaster = new btDefaultVehicleRaycaster(&pDynamicsWorld);

	// �ԗ����쐬����
	m_vehicle = new btRaycastVehicle(m_tuning, CCollision::GetCollision(gameObject)->GetRigidBody(), m_raycaster);
	m_vehicle->setCoordinateSystem(0, 1, 2);
	m_vehicle->resetSuspension();

	// �ԗւ�ǉ�����
	m_vehicle->addWheel(		// �O��
		{ 0.0f, -18.0f, 25.0f },
		{0.0f, -1.0f, 0.0f},
		{-1.0f, 0.0f, 0.0f},
		2.6f,
		10.0f,
		m_tuning,
		true
	);
	m_vehicle->addWheel(		// ���
		{ 0.0f, -18.0f, -25.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		2.6f,
		10.0f,
		m_tuning,
		false
	);

	// �ԗւ̐ݒ�
	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = 20.0f;
		wheel.m_wheelsDampingRelaxation = 2.3f;
		wheel.m_wheelsDampingCompression = 1.0f;
		wheel.m_frictionSlip = 1000.0f;
		wheel.m_rollInfluence = 0.1f;
	}

	// ���[���h�ɒǉ�����
	pDynamicsWorld.addAction(m_vehicle);


	// �^�C���𐶐�����
	m_pFrontWheel = new GameObject;
	m_pFrontWheel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackWheel = new GameObject;
	m_pBackWheel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");

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

	pDynamicsWorld.removeAction(m_vehicle);
	delete m_vehicle;
	m_vehicle = nullptr;

	delete m_raycaster;
	m_raycaster = nullptr;
}

//=============================================================
// [CRaycastVehicle] �X�V
//=============================================================
void CRaycastVehicle::Update()
{
	// �g���N
	float ang = m_body->getAngularVelocity().getZ();
	//m_body->setAngularFactor(btVector3(0.0f, 1.0f, 0.0f));
	m_body->applyTorque(btVector3(ang * -1000000.0f * sinf(transform->GetRotY() - D3DX_PI), 0.0f, ang * -1000000.0f * cosf(transform->GetRotY() - D3DX_PI)));
	m_vehicle->applyEngineForce(4000.0f, 1);

	m_pText->GetComponent<CText>()->SetText(std::to_string(transform->GetRotY()));

	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		btTransform wheelTransform = m_vehicle->getWheelTransformWS(i);

		// �^�C��
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

	// ���C
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



//=============================================================
// [CCar] ������
//=============================================================
void CCar::Init()
{
	// �Ԃ𐶐�����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(25.0f, 15.0f, 50.0f));
	//gameObject->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");
	CCollision::GetCollision(gameObject)->SetMass(500.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ���O�ւ̐���
	m_pFrontLeftTire = new GameObject;
	m_pFrontLeftTire->transform->Translate(-25.0f, -30.0f, 60.0f);
	m_pFrontLeftTire->AddComponent<CCylinderCollider>(10.0f, 3.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontLeftTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pFrontLeftTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pFrontLeftTire)->SetMass(10.0f);
	CCollision::GetCollision(m_pFrontLeftTire)->SetFriction(500);
	m_pFrontLeftTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ���O�ւ̐���
	m_pFrontRightTire = new GameObject;
	m_pFrontRightTire->transform->Translate(25.0f, -30.0f, 60.0f);
	m_pFrontRightTire->AddComponent<CCylinderCollider>(10.0f, 3.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pFrontRightTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pFrontRightTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pFrontRightTire)->SetMass(10.0f);
	CCollision::GetCollision(m_pFrontRightTire)->SetFriction(500);
	m_pFrontRightTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ����ւ̐���
	m_pBackLeftTire = new GameObject;
	m_pBackLeftTire->transform->Translate(-25.0f, -30.0f, -60.0f);
	m_pBackLeftTire->AddComponent<CCylinderCollider>(10.0f, 3.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackLeftTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackLeftTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackLeftTire)->SetMass(10.0f);
	CCollision::GetCollision(m_pBackLeftTire)->SetFriction(500);
	m_pBackLeftTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ����ւ̐���
	m_pBackRightTire = new GameObject;
	m_pBackRightTire->transform->Translate(25.0f, -30.0f, -60.0f);
	m_pBackRightTire->AddComponent<CCylinderCollider>(10.0f, 3.0f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackRightTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackRightTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackRightTire)->SetMass(10.0f);
	CCollision::GetCollision(m_pBackRightTire)->SetFriction(500);
	m_pBackRightTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// �q���W�̐ݒ�
	m_pFrontLeftTire->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(gameObject)->GetRigidBody(),
		CCollision::GetCollision(m_pFrontLeftTire)->GetRigidBody(),
		m_pFrontLeftTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);
	m_pFrontRightTire->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(gameObject)->GetRigidBody(),
		CCollision::GetCollision(m_pFrontRightTire)->GetRigidBody(),
		m_pFrontRightTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);
	m_pBackLeftTire->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(gameObject)->GetRigidBody(),
		CCollision::GetCollision(m_pBackLeftTire)->GetRigidBody(),
		m_pBackLeftTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);
	m_pBackRightTire->AddComponent<CHinge2Constraint>()->SetConstraint(
		CCollision::GetCollision(gameObject)->GetRigidBody(),
		CCollision::GetCollision(m_pBackRightTire)->GetRigidBody(),
		m_pBackRightTire->transform->GetPos(),
		{ 0.0f, 1.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f }
	);

	// �O�ւ̐ݒ�
	auto pFrontLeftHinge = m_pFrontLeftTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pFrontRightHinge = m_pFrontRightTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackLeftHinge = m_pBackLeftTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackRightHinge = m_pBackRightTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	// �h���C�u�G���W��
	pBackLeftHinge->enableMotor(3, true);
	pBackLeftHinge->setMaxMotorForce(3, 40000);
	pBackLeftHinge->setTargetVelocity(3, 0);

	pBackRightHinge->enableMotor(3, true);
	pBackRightHinge->setMaxMotorForce(3, 40000);
	pBackRightHinge->setTargetVelocity(3, 0);

	// �X�e�A�����O�G���W��
	pFrontLeftHinge->enableMotor(5, true);
	pFrontLeftHinge->setMaxMotorForce(5, 5000);
	pFrontLeftHinge->setTargetVelocity(5, 0);

	pFrontRightHinge->enableMotor(5, true);
	pFrontRightHinge->setMaxMotorForce(5, 5000);
	pFrontRightHinge->setTargetVelocity(5, 0);

	// �p�����[�^�[�ݒ�
	pFrontLeftHinge->setParam(BT_CONSTRAINT_CFM, 0.15f, 2);
	pFrontLeftHinge->setParam(BT_CONSTRAINT_ERP, 0.35f, 2);
	pFrontLeftHinge->setDamping(2, 1.0);
	pFrontLeftHinge->setStiffness(2, 80.0);

	pFrontRightHinge->setParam(BT_CONSTRAINT_CFM, 0.15f, 2);
	pFrontRightHinge->setParam(BT_CONSTRAINT_ERP, 0.35f, 2);
	pFrontRightHinge->setDamping(2, 1.0);
	pFrontRightHinge->setStiffness(2, 80.0);


	pFrontLeftHinge->setUpperLimit(D3DX_PI * 0.2f);
	pFrontLeftHinge->setLowerLimit(-D3DX_PI * 0.2f);
	pFrontRightHinge->setUpperLimit(D3DX_PI * 0.2f);
	pFrontRightHinge->setLowerLimit(-D3DX_PI * 0.2f);
	pBackLeftHinge->setUpperLimit(D3DX_PI * 0.08f);
	pBackLeftHinge->setLowerLimit(-D3DX_PI * 0.08f);
	pBackRightHinge->setUpperLimit(D3DX_PI * 0.08f);
	pBackRightHinge->setLowerLimit(-D3DX_PI * 0.08f);
}

//=============================================================
// [CCar] �I��
//=============================================================
void CCar::Uninit()
{
}

//=============================================================
// [CCar] �X�V
//=============================================================
void CCar::Update()
{
	// 2���q���W���擾����
	auto pFrontLeftHinge = m_pFrontLeftTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pFrontRightHinge = m_pFrontRightTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackLeftHinge = m_pBackLeftTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	auto pBackRightHinge = m_pBackRightTire->GetComponent<CHinge2Constraint>()->GetHinge2();

	// �A�N�Z��
	if (INPUT_INSTANCE->onInput("accel"))
	{
		m_fEngineForce += 50.0f;
	}
	m_fEngineForce += (0.0f - m_fEngineForce) * 0.002f;
	pBackLeftHinge->setTargetVelocity(3, -m_fEngineForce);
	pBackRightHinge->setTargetVelocity(3, -m_fEngineForce);


	// �����]��
	if (INPUT_INSTANCE->onPress("a"))
	{
		m_fSteering += 0.5f;
	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		m_fSteering -= 0.5f;
	}
	m_fSteering += (0.0f - m_fSteering) * 0.08f;

	pFrontLeftHinge->setTargetVelocity(5, m_fSteering);
	pFrontRightHinge->setTargetVelocity(5, m_fSteering);
	pBackLeftHinge->setTargetVelocity(5, m_fSteering);
	pBackRightHinge->setTargetVelocity(5, m_fSteering);
}