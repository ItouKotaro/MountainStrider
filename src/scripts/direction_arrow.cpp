//=============================================================
//
// �Q�[���I�u�W�F�N�g [direction_arrow.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "direction_arrow.h"
#include "scene/game.h"

//=============================================================
// [DirectionArrow] ������
//=============================================================
void DirectionArrow::Init()
{
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);

	// �o�C�N���擾����
	m_vehicle = gameScene->GetBike();

	// ���̃��f���̏�����
	m_arrowModel = new SingleComponent<CMesh>();
	m_arrowModel->Init();
	m_arrowModel->LoadMeshX("data\\MODEL\\direction_arrow.x");
	m_arrowModel->SetParent(gameObject);
}

//=============================================================
// [DirectionArrow] �I��
//=============================================================
void DirectionArrow::Uninit()
{
	// ���̃��f���̔j��
	if (m_arrowModel != nullptr)
	{
		m_arrowModel->Uninit();
		delete m_arrowModel;
		m_arrowModel = nullptr;
	}
}

//=============================================================
// [DirectionArrow] �X�V
//=============================================================
void DirectionArrow::Update()
{
	// �o�C�N�̈ʒu�Ɋ�Â��Ė��̈ʒu�����߂�
	D3DXVECTOR3 pos;
	pos = m_vehicle->transform->GetWPos();
	pos.y = m_fadeY;
	gameObject->transform->SetPos(pos);

	// ��]���������߂�
	m_angle += (Benlib::PosAngle(m_vehicle->transform->GetWPos(), m_destination) - m_angle) * 0.08f;
	gameObject->transform->Translate(sinf(m_angle) * 50.0f, 0.0f, cosf(m_angle) * 50.0f);
	gameObject->transform->SetRot(0.0f, m_angle, 0.0f);

	// �������č������m���߂�
	btVector3 Start = btVector3(gameObject->transform->GetWPos().x, m_vehicle->transform->GetWPos().y + 20.0f, gameObject->transform->GetWPos().z);
	btVector3 End = btVector3(gameObject->transform->GetWPos().x, m_vehicle->transform->GetWPos().y - 50.0f, gameObject->transform->GetWPos().z);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // �q�b�g�����Ƃ�
		m_fadeY += ((RayCallback.m_hitPointWorld.getY() + 5.0f) - m_fadeY) * 0.08f;
	}

	// ���̃��f���̍X�V
	m_arrowModel->Update();
}

//=============================================================
// [DirectionArrow] �`��
//=============================================================
void DirectionArrow::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// �f�o�C�X���擾

	// ���̃��f���̕`��
	m_arrowModel->SetShader(GetShaderInfo());
	m_arrowModel->Draw();
}