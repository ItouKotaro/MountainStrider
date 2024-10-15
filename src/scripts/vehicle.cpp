//=============================================================
//
// �ԗ� [vehicle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle.h"

//=============================================================
// [CVehicle] ������
//=============================================================
void CVehicle::Init()
{
	// ���̂̌`��
	btBoxShape* bodyShape = new btBoxShape(btVector3(10.0f, 2.0f, 20.0f));
	
	CPhysics::GetInstance()->GetCollisionShape().push_back(bodyShape);

	// �ʒu��ݒ�
	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

	// �������[�����g
	btVector3 inertia(0, 0, 0);
	bodyShape->calculateLocalInertia(btScalar(10.0f), inertia);

	// ���̑���
	m_pMotionState = new btDefaultMotionState(startTransform);

	// ���̍쐬
	btRigidBody::btRigidBodyConstructionInfo rb_info(btScalar(10.0f), m_pMotionState, bodyShape, inertia);

	// �ԑ̂�ݒ肷��
	m_bodyRigidbody = new btRigidBody(rb_info);
	CPhysics::GetInstance()->GetDynamicsWorld().addRigidBody(m_bodyRigidbody);

	// �u���[�h�t�F�C�Y
	//CPhysics::GetInstance()->GetDynamicsWorld().getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_bodyRigidbody->getBroadphaseHandle(), CPhysics::GetInstance()->GetDynamicsWorld().getDispatcher());

	// ���C�L���X�^�[
	m_vehicleRaycaster = new btDefaultVehicleRaycaster(&CPhysics::GetInstance()->GetDynamicsWorld());

	// �ԗ��A�N�V�����C���^�[�t�F�C�X���쐬����
	m_vehicle = new btRaycastVehicle(m_vehicleTuning, m_bodyRigidbody, m_vehicleRaycaster);
	m_bodyRigidbody = m_vehicle->getRigidBody();
	m_bodyRigidbody->setActivationState(DISABLE_DEACTIVATION);

	// �ǉ�����
	CPhysics::GetInstance()->GetDynamicsWorld().addVehicle(m_vehicle);
	//m_vehicle->setCoordinateSystem(0, 1, 2);

	btVector3 wheelDirectionCS0(0, -1, 0);
	btVector3 wheelAxleCS(-1, 0, 0);
	float wheelWidth = 10.0f;
	float wheelRadius = 10.0f;
	float wheelFriction = 2.0f;
	float suspensionStiffness = 20.f;
	float suspensionDamping = 2.3f;
	float suspensionCompression = 4.4f;
	float rollInfluence = 0.01f;  //1.0f;
	float connectionHeight = -0.9f;

	// �ԗւ�ǉ�����
	btVector3 connectionPointCS0(0.0f, connectionHeight, 14.0f - 2.8f);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6f, wheelRadius, m_vehicleTuning, true);

	connectionPointCS0 = btVector3(0.0f, connectionHeight, -14.0f + 2.8f);
	m_vehicle->addWheel(connectionPointCS0, wheelDirectionCS0, wheelAxleCS, 0.6f, wheelRadius, m_vehicleTuning, false);

	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		btWheelInfo& wheel = m_vehicle->getWheelInfo(i);
		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}
}

//=============================================================
// [CVehicle] �I��
//=============================================================
void CVehicle::Uninit()
{
	// �������[���h�������
	CPhysics::GetInstance()->GetDynamicsWorld().removeVehicle(m_vehicle);
	if (m_vehicle != nullptr)
	{
		delete m_vehicle;
		m_vehicle = nullptr;
	}

	// ���[�V�����X�e�[�g��j������
	if (m_pMotionState != nullptr)
	{
		delete m_pMotionState;
		m_pMotionState = nullptr;
	}

	// ���C�L���X�^�[��j������
	if (m_vehicleRaycaster != nullptr)
	{
		delete m_vehicleRaycaster;
		m_vehicleRaycaster = nullptr;
	}

	// ���W�b�h�{�f�B��j������
	CPhysics::GetInstance()->GetDynamicsWorld().removeRigidBody(m_bodyRigidbody);
	if (m_bodyRigidbody != nullptr)
	{
		delete m_bodyRigidbody;
		m_bodyRigidbody = nullptr;
	}
}

//=============================================================
// [CVehicle] �X�V
//=============================================================
void CVehicle::Update()
{
	for (int i = 0; i < 2; ++i)
	{
		if (m_vehicle)
		{
			btTransform wheelTransform = m_vehicle->getWheelTransformWS(i);
			//m_raycastVehicle->updateVehicle(1. / 60.);
		}
	}

	for (int i = 0; i < m_vehicle->getNumWheels(); i++)
	{
		m_vehicle->updateWheelTransform(i, true);
	}


	for (int i = 0; i < 2; ++i)
	{
		int aa = m_vehicle->getNumWheels();
		m_vehicle->applyEngineForce(1000.0f, i);
		//m_vehicle->setBrake(100.0f, i);
	}
}