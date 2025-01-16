//=============================================================
//
// �����C�x���g [tornado.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "tornado.h"
#include "manager.h"
#include "component/3d/particle.h"

//=============================================================
// �g���l�[�h�p�p�[�e�B�N��
//=============================================================
class TornadoParticle : public ParticleModule::Shape
{
public:
	ResultData GetResult()
	{
		ResultData data;
		
		// �ʒu
		float height = Benlib::RandomFloat(0.0f, 500.0f);
		float radius = (height * height) / 10.0f;
		float angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);

		data.position.x = sinf(angle) * radius;
		data.position.y = height;
		data.position.z = cosf(angle) * radius;
	}
};

//=============================================================
// [TornadoEvent] ������
//=============================================================
void TornadoEvent::Init()
{
	// �p�[�e�B�N�����쐬����
	m_tornade = new GameObject();
	m_tornade->AddComponent<ParticleSystem>();
}

//=============================================================
// [TornadoEvent] �I��
//=============================================================
void TornadoEvent::Uninit()
{

}

//=============================================================
// [TornadoEvent] �X�V
//=============================================================
void TornadoEvent::Update()
{
	// �^�C�}�[��i�߂�
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();

	// �C�x���g�^�C�}�[���I�������Ƃ�
	if (m_eventTimer <= 0.0f)
	{
		// �C�x���g���I������
		EndEvent();
	}
}