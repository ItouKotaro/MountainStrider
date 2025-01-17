//=============================================================
//
// �����C�x���g [tornado.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "tornado.h"
#include "manager.h"
#include "component/3d/particle.h"
#include "scripts/destructible.h"
#include "scripts/vehicle.h"

#include "scene/game.h"

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
		float height = Benlib::RandomFloat(0.0f, TORNADO_HEIGHT);
		float radius = (height * height) / 100.0f;
		float angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);

		data.position.x = sinf(angle) * radius;
		data.position.y = height;
		data.position.z = cosf(angle) * radius;

		// ����
		data.direction = { 0.0f, 1.0f, 0.0f };

		return data;
	}
private:
	const float TORNADO_HEIGHT = 100.0f;
};

//=============================================================
// [TornadoEvent] ������
//=============================================================
void TornadoEvent::Init()
{
	// �g���l�[�h���쐬����
	m_tornade = new GameObject();
	m_tornade->transform->SetPos(GameObject::Find("Vehicle")->transform->GetWPos());

	auto particle = m_tornade->AddComponent<ParticleSystem>();
	particle->SetShape(new TornadoParticle());
	particle->SetVortex(0.1f);
	particle->GetEmission()->SetRateOverTime(150.0f);
	particle->SetPower(0.5f, 1.0f);
	particle->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\smoke00.png");
	particle->SetSize(15.0f, 30.0f);
	particle->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f));

	// �n�`�̍��x�����擾����
	Terrain* terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();
	m_maxHeight = terrain->GetMaxHeight();
	m_minHeight = terrain->GetMinHeight();

	// �C�x���g���Ԃ�ݒ肷��
	m_eventTimer = EVENT_BASE_TIME + Benlib::RandomFloat(0.0f, EVENT_RANDOM_TIME);
}

//=============================================================
// [TornadoEvent] �I��
//=============================================================
void TornadoEvent::Uninit()
{
	// �g���l�[�h��j������
	m_tornade->Destroy();
}

//=============================================================
// [TornadoEvent] �X�V
//=============================================================
void TornadoEvent::Update()
{
	// �ړ��̍X�V
	UpdateMove();

	// �A�N�V�����̍X�V
	UpdateAction();

	// �^�C�}�[��i�߂�
	m_moveTimer -= CManager::GetInstance()->GetDeltaTime();
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();

	// �ړ��^�C�}�[���I�������Ƃ�
	if (m_moveTimer <= 0.0f)
	{
		ResetMoveAngle();
	}

	// �C�x���g�^�C�}�[���I�������Ƃ�
	if (m_eventTimer <= 0.0f)
	{
		// �C�x���g���I������
		EndEvent();
	}
}

//=============================================================
// [TornadoEvent] ���������������Z�b�g����
//=============================================================
void TornadoEvent::ResetMoveAngle()
{
	// ���������߂�
	m_moveAngle += Benlib::RandomFloat(-MOVE_ANGLE_RANGE, MOVE_ANGLE_RANGE);

	// ���Ԃ����߂�
	m_moveTimer = MOVE_BASE_TIME + Benlib::RandomFloat(0.0f, MOVE_RANDOM_TIME);
}

//=============================================================
// [TornadoEvent] �ړ��̍X�V
//=============================================================
void TornadoEvent::UpdateMove()
{
	// �ړ�������
	m_tornade->transform->Translate(sinf(m_moveAngle) * TORNADO_SPEED, 0.0f, cosf(m_moveAngle) * TORNADO_SPEED);

	// �����𒲐߂���
	btVector3 Start = btVector3(m_tornade->transform->GetWPos().x, m_maxHeight + 10.0f, m_tornade->transform->GetWPos().z);
	btVector3 End = Start + btVector3(0.0f, m_minHeight - 10.0f, 0.0f);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit() && CCollision::GetGameObjectFromCollisionObj(RayCallback.m_collisionObject)->GetName() == "TerrainField")
	{ // �n�ʂɐڐG�����Ƃ�
		m_tornade->transform->SetPos(m_tornade->transform->GetPos().x, RayCallback.m_hitPointWorld.getY(), m_tornade->transform->GetPos().z);
	}
}

//=============================================================
// [TornadoEvent] �A�N�V�����̍X�V
//=============================================================
void TornadoEvent::UpdateAction()
{
	// ���͂̃I�u�W�F�N�g�𐁂���΂�
	std::vector<GameObject*> allObjects = GameObject::GetAllGameObjects();
	for (auto itr = allObjects.begin(); itr != allObjects.end(); itr++)
	{
		if (Benlib::PosPlaneDistance((*itr)->transform->GetWPos(), m_tornade->transform->GetWPos()) < TORNADO_RANGE &&
			(*itr)->transform->GetWPos().y <= m_tornade->transform->GetWPos().y + TORNADO_HEIGHT)
		{
			// �I�u�W�F�N�g
			if ((*itr)->GetComponent<Destructible>() != nullptr)
			{
				(*itr)->GetComponent<Destructible>()->ForceHit();

				// �g���l�[�h�Ɋ������܂���
				D3DXVECTOR3 dir = (m_tornade->transform->GetWPos() - (*itr)->transform->GetWPos()) * 0.5f;
				dir.y = 3.0f;
				CCollision::GetCollision(*itr)->GetRigidBody()->applyCentralForce(btVector3(dir.x, dir.y, dir.z) * 350.0f);
			}

			// �o�C�N
			if ((*itr)->GetComponent<CVehicle>() != nullptr)
			{
				(*itr)->GetComponent<CVehicle>()->AddDamage(0.3f);

				// �g���l�[�h�Ɋ������܂���
				D3DXVECTOR3 dir = (m_tornade->transform->GetWPos() - (*itr)->transform->GetWPos());
				dir.y = 5.0f;
				CCollision::GetCollision(*itr)->GetRigidBody()->applyCentralForce(btVector3(dir.x, dir.y, dir.z) * 8000.0f);
			}
		}
	}
}