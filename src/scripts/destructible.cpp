//=============================================================
//
// �j��\ [destructible.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "destructible.h"
#include "manager.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scene/game.h"
#include "vehicle.h"

const float Destructible::FADE_VALUE = 0.03f;
const float Destructible::START_TIME = 3.5f;

//=============================================================
// [Destructible] ������
//=============================================================
void Destructible::Init()
{
	CCollision* collision = CCollision::GetCollision(gameObject);
	if (collision != nullptr)
	{
		collision->IsTrigger(true);
	}
}

//=============================================================
// [Destructible] �X�V
//=============================================================
void Destructible::Update()
{
	if (static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetPause()->GetPause())
		return;

	if (m_isHit)
	{
		// �J�n���Ԃ�0�ɂȂ�܂ł͏����Ȃ�
		if (m_startTimer > 0.0f)
		{
			m_startTimer -= CManager::GetInstance()->GetDeltaTime();
			return;
		}

		// ���b�V�����X�g���擾����
		std::vector<GameObject*> pMeshList = gameObject->GetChildren(true);
		for (auto itr = pMeshList.begin(); itr != pMeshList.end(); itr++)
		{
			CMesh* mesh = (*itr)->GetComponent<CMesh>();
			if (mesh != nullptr)
			{
				mesh->SetAlpha(m_fade);
			}
		}

		// �t�F�[�h����
		if (m_fade > 0.0f)
		{
			m_fade -= FADE_VALUE;
		}
		else
		{ // �I��
			gameObject->Destroy(true);
			return;
		}
	}
}

//=============================================================
// [Destructible] �Փ�
//=============================================================
void Destructible::OnTriggerEnter(GameObject* other)
{
	if (m_isHit) return;

	// �o�C�N�R���|�[�l���g���擾����
	CVehicle* vehicle = other->GetComponent<CVehicle>();

	// �o�C�N�Ƃ̏Փˏ���
	if (vehicle != nullptr)
	{
		// �o�C�N�Ƀ_���[�W��^����
		vehicle->AddDamage(m_decoData->type->damage);

		// ���W�b�h�{�f�B�̒ǉ�
		CCollision* collision = CCollision::GetCollision(gameObject);
		if (collision != nullptr)
		{
			collision->IsTrigger(false);
			gameObject->AddComponent<CRigidBody>();
		}

		// �f�R���[�V�����f�[�^����폜����
		if (m_decoData != nullptr)
		{
			m_decoManager->RemoveData(m_decoData);
		}

		// �J�n���Ԃ�ݒ肷��
		m_startTimer = START_TIME;

		m_isHit = true;
	}
}