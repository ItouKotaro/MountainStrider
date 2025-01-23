//=============================================================
//
// �ԗ��p�[�e�B�N�� [vehicle_particle.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_particle.h"
#include "scripts/trajectory.h"
#include "component/3d/mesh.h"
#include "component/3d/collision.h"
#include "scripts/wreckage.h"
#include "manager.h"
#include "scene/game.h"

//=============================================================
// [VehicleParticle] ������
//=============================================================
void VehicleParticle::Init()
{
	// �o�C�N���擾����
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// �n�`���擾����
	m_terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();

	// �r���擾����
	m_lake = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetLake();


	// �O�Ղ̒ǉ�
	m_trajectory = new GameObject();
	m_trajectory->AddComponent<CTrajectory>()->SetShow(true);

	// ����p�[�e�B�N���̏�����
	m_backParticleTimer = 0.0f;
}

//=============================================================
// [VehicleParticle] �I��
//=============================================================
void VehicleParticle::Uninit()
{
}

//=============================================================
// [VehicleParticle] �X�V
//=============================================================
void VehicleParticle::Update()
{
	// �r���擾����
	if (m_lake == nullptr)
		m_lake = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetLake();

	// �O�Ղ̍X�V
	UpdateTrajectory();

	// ����p�[�e�B�N���̍X�V
	UpdateBackParticle();
}

//=============================================================
// [VehicleParticle] �`��
//=============================================================
void VehicleParticle::Draw()
{
}

//=============================================================
// [VehicleParticle] �O�Ղ̍X�V
//=============================================================
void VehicleParticle::UpdateTrajectory()
{
	auto trajectory = m_trajectory->GetComponent<CTrajectory>();

	D3DXMATRIX mtx;
	mtx = transform->GetMatrix();
	D3DXVECTOR3 pos0, pos1;
	pos0 = { -0.5f, -1.0f, 7.0f };
	pos1 = { 0.5f, -1.0f, 7.0f };
	D3DXVec3TransformCoord(&pos0, &pos0, &mtx);
	D3DXVec3TransformCoord(&pos1, &pos1, &mtx);

	trajectory->AddTrajectory(pos0, pos1);
}

//=============================================================
// [VehicleParticle] ����p�[�e�B�N���̍X�V
//=============================================================
void VehicleParticle::UpdateBackParticle()
{
	if (!m_vehicle || !m_lake) return;

	// �N�[���^�C�����I����Ă��Ȃ��Ƃ�
	if (m_backParticleTimer > 0.0f)
	{
		m_backParticleTimer -= CManager::GetInstance()->GetDeltaTime();
		return;
	}

	// �󒆂ɕ�����ł���Ƃ�
	if (m_vehicle->GetFlying())
		return;

	// �r�̒��̂Ƃ�
	if (m_lake->IsEnabled() && m_vehicle->transform->GetWPos().y < m_lake->GetHeight() - BG_LAKE_SPACE)
		return;

	// �����ɂ���ăN�[���_�E���^�C����ύX����
	float speed = m_vehicle->GetSpeed();

	if (speed < BP_MIN_GENERATE) return;
	else if (speed < 25.0f) m_backParticleTimer = 0.4f;
	else if (speed < 35.0f) m_backParticleTimer = 0.3f;
	else if (speed < 50.0f) m_backParticleTimer = 0.2f;
	else if (speed < 60.0f) m_backParticleTimer = 0.15f;
	else if (speed < 80.0f) m_backParticleTimer = 0.1f;
	else m_backParticleTimer = 0.05f;

	// �����_���v�f
	m_backParticleTimer += Benlib::RandomFloat(-BP_RANDOM_TIME, BP_RANDOM_TIME);

	// �{�b�N�X�𐶐�����
	GameObject* boxObj = new GameObject("VehicleParticle", "Particle");

	// �����ʒu�����߂�
	D3DXVECTOR3 generatePos = { Benlib::RandomFloat(-BP_RANDOM_WIDTH, BP_RANDOM_WIDTH), 0.0f, BP_BACK_RANGE };
	D3DXMATRIX mtx = transform->GetMatrix();
	D3DXVec3TransformCoord(&generatePos, &generatePos, &mtx);
	boxObj->transform->SetPos(generatePos);

	// �T�C�Y�����߂�
	float size = Benlib::RandomFloat(BP_MIN_SIZE, BP_MIN_SIZE + speed * BP_SPEED_RATE);
	boxObj->transform->SetScale(size);

	// ��]�����߂�
	boxObj->transform->SetRot(Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f), Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f), 0.0f);

	// ���݈ʒu����F�����߂�
	auto heightColor = m_terrain->GetHeightColor(transform->GetWPos().y);

	// �r�̏ꍇ�͐F��ύX����
	if (m_lake->IsEnabled() && m_vehicle->transform->GetWPos().y < m_lake->GetHeight() + BG_LAKE_TOP)
	{
		heightColor = m_lake->GetBaseColor();
	}

	// ���̂𐶐�����
	boxObj->AddComponent<CMesh>()->LoadMeshX("data\\MODEL\\vehicle_box.x");
	boxObj->GetComponent<CMesh>()->SetColor(heightColor * 0.7f);
	boxObj->GetComponent<CMesh>()->SetCustomColor(true);
	boxObj->AddComponent<CBoxCollider>(D3DXVECTOR3(size, size, size) * 2.0f);
	boxObj->AddComponent<CRigidBody>()->GetRigidBody()->setLinearVelocity(
		btVector3(sinf(transform->GetRot().y +D3DX_PI *0.5f) * Benlib::RandomFloat(-BP_EXPANSE_VALUE, BP_EXPANSE_VALUE),
			Benlib::RandomFloat(30.0f, 30.0f + speed * 0.2f),
			cosf(transform->GetRot().y + D3DX_PI * 0.5f) * Benlib::RandomFloat(-BP_EXPANSE_VALUE, BP_EXPANSE_VALUE))
	);
	boxObj->GetComponent<CRigidBody>()->GetRigidBody()->setGravity(btVector3(0.0f, BP_GRAVITY, 0.0f));
	boxObj->AddComponent<CWreckage>();
}