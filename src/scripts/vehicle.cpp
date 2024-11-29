//=============================================================
//
// �ԗ� [vehicle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle.h"
#include "manager.h"

#include "component/3d/collision.h"
#include "component/3d/mesh.h"
#include "component/2d/text.h"
#include "benlib.h"

#include "scripts/status_ui.h"
#include "scene/game.h"

const float CVehicle::ENGINEFORCE_VALUE = 60.0f;
const float CVehicle::STEERING_VALUE = 10.0f;
const float CVehicle::MIN_ENGINEFORCE_VALUE = 10.0f;
const float CVehicle::MAX_ENGINEFORCE = 600000.0f;
const float CVehicle::MAX_STEERING = 50000.0f;
const float CVehicle::MAX_FUEL = 4000.0f;
const float CVehicle::MAX_ENDURANCE = 300.0f;

//=============================================================
// [CVehicle] ������
//=============================================================
void CVehicle::Init()
{
	// �ϐ��̏�����
	m_measureCounter = 0;
	m_measurePos = transform->GetWPos();
	m_fuel = MAX_FUEL;
	m_endurance = MAX_ENDURANCE;
	m_pStatusUI = nullptr;

	// �o�C�N�𐶐�����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(6.0f, 6.0f, 20.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	CCollision::GetCollision(gameObject)->SetMass(400.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();
	gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, -180.0f, 0.0f));

	// �ԑ�
	GameObject* pBodyModel = new GameObject;
	pBodyModel->SetParent(gameObject);
	pBodyModel->transform->Translate(0.0f, 25.0f, 0.0f);
	pBodyModel->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\body.x");

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
	CCollision::GetCollision(m_pFrontTire)->SetMass(30.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(900);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ��ւ̐���
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -5.0f, 20.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(10.0f, 3.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackTire)->SetMass(30.0f);
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
	pBackHinge->setMaxMotorForce(3, MAX_ENGINEFORCE);
	pBackHinge->setTargetVelocity(3, 0);

	// �X�e�A�����O�G���W��
	pFrontHinge->enableMotor(5, true);
	pFrontHinge->setMaxMotorForce(5, MAX_STEERING);
	pFrontHinge->setTargetVelocity(5, 0);

	// ���̑��̐ݒ�
	pFrontHinge->enableSpring(2, true);
	pBackHinge->enableSpring(2, true);
	pFrontHinge->setStiffness(2, 5.0f);
	pBackHinge->setStiffness(2, 5.0f);
	pFrontHinge->setDamping(2, 0.00f);
	pBackHinge->setDamping(2, 0.00f);

	// ��]����
	pFrontHinge->setUpperLimit(D3DX_PI* 0.06f);
	pFrontHinge->setLowerLimit(D3DX_PI * -0.06f);
	pBackHinge->setUpperLimit(0.0f);
	pBackHinge->setLowerLimit(0.0f);
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
	// ���쏈��
	ControlVehicle();

	// ���x���v�Z����
	UpdateSpeedMeter();

	// �X�e�[�^�XUI�̍X�V
	UpdateStatusUI();

	// �Q�[���I�[�o�[����
	if (m_fuel <= 0.0f)
	{ // �R���������Ȃ����Ƃ�
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetCurrentScene()->pScene)->onGameOver();
	}
}

//=============================================================
// [CVehicle] �o�C�N�̑���
//=============================================================
void CVehicle::SetPos(const D3DXVECTOR3& pos)
{
	gameObject->transform->SetPos(pos);
	m_pFrontTire->transform->SetPos(pos);
	m_pBackTire->transform->SetPos(pos);
}

//=============================================================
// [CVehicle] �_���[�W��^����
//=============================================================
void CVehicle::AddDamage(const float& value)
{
	// �ϋv�l�����炷
	m_endurance -= value;

	// �ϋv�l�������Ȃ����Ƃ��̏���
	if (m_endurance <= 0)
	{
		// �Q�[���I�[�o�[����
		static_cast<CGameScene*>(CSceneManager::GetInstance()->GetCurrentScene()->pScene)->onGameOver();
	}
}

//=============================================================
// [CVehicle] �R���𑝂₷
//=============================================================
void CVehicle::AddFuel(const float& value)
{
	m_fuel = m_fuel + value > MAX_FUEL ? MAX_FUEL : m_fuel + value;
}

//=============================================================
// [CVehicle] �o�C�N�̑���
//=============================================================
void CVehicle::ControlVehicle()
{
	// �N���オ������Ƀg���N��������
	float ang = transform->GetWRotZ();
	D3DXVECTOR3 angularVelocity = {
		sinf(transform->GetWRotY()) * -ang * 1.5f,
		0.0f,
		cosf(transform->GetWRotY()) * -ang * 1.5f
	};

	// �A�N�Z��
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	if (m_fuel > 0.0f)
	{ // �R��������Ƃ�
		// �G���W���͂��擾����
		float fEngineForce = INPUT_INSTANCE->onInput("accel") ? ENGINEFORCE_VALUE : MIN_ENGINEFORCE_VALUE;

		// �^�C������]������
		pBackHinge->setTargetVelocity(3, fEngineForce);

		// �R�������炷
		m_fuel -= fEngineForce * 0.01f;
	}
	else
	{ // �R�����Ȃ��Ƃ�
		// �^�C���̉�]���~�߂�
		pBackHinge->setTargetVelocity(3, 0.0f);
	}
	
	// �����]��
	float fSteeringVelocity = 0.0f;
	if (INPUT_INSTANCE->onPress("a"))
	{
		angularVelocity += {sinf(transform->GetRotY()) * 0.8f, 0.0f, cosf(transform->GetRotY()) * 0.8f};
		fSteeringVelocity += STEERING_VALUE;

	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		angularVelocity += {sinf(transform->GetRotY()) * -0.8f, 0.0f, cosf(transform->GetRotY()) * -0.8f};
		fSteeringVelocity -= STEERING_VALUE;
	}
	auto pFrontHinge = m_pFrontTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	pFrontHinge->setTargetVelocity(5, fSteeringVelocity);

	// �X������
	if (INPUT_INSTANCE->onPress("w"))
	{
		angularVelocity += {sinf(transform->GetRotY() + D3DX_PI * 0.5f) * 0.8f, 0.0f, cosf(transform->GetRotY() + D3DX_PI * 0.5f) * 0.8f};
	}
	if (INPUT_INSTANCE->onPress("s"))
	{
		angularVelocity += {sinf(transform->GetRotY() + D3DX_PI * 0.5f) * -0.8f, 0.0f, cosf(transform->GetRotY() + D3DX_PI * 0.5f) * -0.8f};
	}

	// �X�����x��K�p����
	CCollision::GetCollision(gameObject)->GetRigidBody()->setAngularVelocity(btVector3(angularVelocity.x, angularVelocity.y, angularVelocity.z));
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
}

//=============================================================
// [CVehicle] �X�e�[�^�XUI�̍X�V
//=============================================================
void CVehicle::UpdateStatusUI()
{
	if (m_pStatusUI != nullptr)
	{
		// �R�������X�V����
		float fuelPercent = m_fuel / MAX_FUEL;
		m_pStatusUI->SetFuel(fuelPercent);

		// �ϋv�l�����X�V����
		float endurancePercent = m_endurance / MAX_ENDURANCE;
		m_pStatusUI->SetEndurance(endurancePercent);
	}
}