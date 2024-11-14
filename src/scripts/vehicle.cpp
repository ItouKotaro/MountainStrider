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

#include "scripts/status_ui.h"

const float CVehicle::ENGINEFORCE_VALUE = 50.0f;
const float CVehicle::STEERING_VALUE = 10.0f;
const float CVehicle::MIN_ENGINEFORCE_VALUE = 10.0f;
const float CVehicle::MAX_ENGINEFORCE = 300000.0f;
const float CVehicle::MAX_STEERING = 50000.0f;
const float CVehicle::MAX_FUEL = 500.0f;

//=============================================================
// [CVehicle] ������
//=============================================================
void CVehicle::Init()
{
	// �ϐ��̏�����
	m_measureCounter = 0;
	m_measurePos = transform->GetWPos();
	m_fFuel = MAX_FUEL;

	// �o�C�N�𐶐�����
	gameObject->AddComponent<CBoxCollider>(D3DXVECTOR3(5.0f, 10.0f, 20.0f), D3DXVECTOR3(0.0f, 10.0f, 0.0f));
	CCollision::GetCollision(gameObject)->SetMass(250.0f);
	gameObject->AddComponent<CRigidBody>();
	gameObject->GetComponent<CRigidBody>()->EnableAlwayActive();
	gameObject->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, -120.0f, 0.0f));

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
	CCollision::GetCollision(m_pFrontTire)->SetMass(60.0f);
	CCollision::GetCollision(m_pFrontTire)->SetFriction(10);
	m_pFrontTire->GetComponent<CRigidBody>()->EnableAlwayActive();

	// ��ւ̐���
	m_pBackTire = new GameObject;
	m_pBackTire->transform->Translate(0.0f, -5.0f, 20.0f);
	m_pBackTire->AddComponent<CCylinderCollider>(10.0f, 3.5f, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, D3DX_PI * 0.5f));
	m_pBackTire->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\MOTOR_BIKE\\tire.x");
	m_pBackTire->AddComponent<CRigidBody>();
	CCollision::GetCollision(m_pBackTire)->SetMass(40.0f);
	CCollision::GetCollision(m_pBackTire)->SetFriction(10);
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
	pFrontHinge->setStiffness(2, 0.1f);
	pBackHinge->setStiffness(2, 0.1f);
	pFrontHinge->setDamping(2, 0.01f);
	pBackHinge->setDamping(2, 0.01f);

	// ��]����
	pFrontHinge->setUpperLimit(D3DX_PI * 0.15f);
	pFrontHinge->setLowerLimit(-D3DX_PI * 0.15f);
	pBackHinge->setUpperLimit(0.0f);
	pBackHinge->setLowerLimit(0.0f);

	// ��ԃe�L�X�g
	m_pSpeedText = new GameObject;
	m_pSpeedText->AddComponent<CText>();
	m_pSpeedText->GetComponent<CText>()->SetFontSize(50.0f);

	// �X�e�[�^�XUI
	m_pStatusUI = new GameObject("StatusUI", "UI");
	m_pStatusUI->AddComponent<CStatusUI>();

}

//=============================================================
// [CVehicle] �I��
//=============================================================
void CVehicle::Uninit()
{
	// ��ԃe�L�X�g�̔j��
	m_pSpeedText->Destroy();

	// �X�e�[�^�XUI�̔j��
	m_pStatusUI->Destroy();
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
}

//=============================================================
// [CVehicle] �o�C�N�̑���
//=============================================================
void CVehicle::ControlVehicle()
{
	// �N���オ������Ƀg���N��������
	float ang = transform->GetWRotZ();
	D3DXVECTOR3 angularVelocity = {
		sinf(transform->GetWRotY()) * -ang * 0.9f,
		CCollision::GetCollision(gameObject)->GetRigidBody()->getAngularVelocity().getY() * 0.08f,
		cosf(transform->GetWRotY()) * -ang * 0.9f
	};

	// �A�N�Z��
	auto pBackHinge = m_pBackTire->GetComponent<CHinge2Constraint>()->GetHinge2();
	if (m_fFuel > 0.0f)
	{ // �R��������Ƃ�
		// �G���W���͂��擾����
		float fEngineForce = INPUT_INSTANCE->onInput("accel") ? ENGINEFORCE_VALUE : MIN_ENGINEFORCE_VALUE;

		// �^�C������]������
		pBackHinge->setTargetVelocity(3, fEngineForce);

		// �R�������炷
		m_fFuel -= fEngineForce * 0.01f;
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

	// �󋵂�\������
	m_pSpeedText->GetComponent<CText>()->SetText("���x: " + std::to_string(m_fSpeed) + "| �R��: " + std::to_string(m_fFuel));
}

//=============================================================
// [CVehicle] �X�e�[�^�XUI�̍X�V
//=============================================================
void CVehicle::UpdateStatusUI()
{
	auto pStatusUI = m_pStatusUI->GetComponent<CStatusUI>();

	// �R�������X�V����
	float fFuelPercent = m_fFuel / MAX_FUEL;
	pStatusUI->SetFuel(fFuelPercent);
}