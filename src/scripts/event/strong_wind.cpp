//=============================================================
//
// ���� [strong_wind.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "strong_wind.h"
#include "manager.h"

//=============================================================
// [StrongWindEvent] ������
//=============================================================
void StrongWindEvent::Init()
{
	// �o�C�N���擾����
	m_vehicle = GameObject::Find("Vehicle");

	// �����������߂�
	m_angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);
	
	// �͂����߂�
	m_power = Benlib::RandomFloat(MIN_POWER, MAX_POWER);

	// �C�x���g���Ԃ�ݒ肷��
	m_eventTimer = EVENT_BASE_TIME + Benlib::RandomFloat(0.0f, EVENT_RANDOM_TIME);
}

//=============================================================
// [StrongWindEvent] �I��
//=============================================================
void StrongWindEvent::Uninit()
{

}

//=============================================================
// [StrongWindEvent] �X�V
//=============================================================
void StrongWindEvent::Update()
{
	// �͂�������
	CCollision::GetCollision(m_vehicle)->GetRigidBody()->applyCentralForce(
		btVector3(sinf(m_angle) * m_power, -m_power * 0.5f, cosf(m_angle) * m_power)
	);

	CCollision::GetCollision(m_vehicle)->GetRigidBody()->setAngularVelocity(
		btVector3(sinf(m_angle) * m_power * 2.0f, 0.0f, cosf(m_angle) * m_power * 2.0f)
	);

	// �^�C�}�[��i�߂�
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();

	// �C�x���g�^�C�}�[���I�������Ƃ�
	if (m_eventTimer <= 0.0f)
	{
		// �C�x���g���I������
		EndEvent();
	}
}