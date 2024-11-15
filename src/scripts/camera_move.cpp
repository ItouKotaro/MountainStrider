//=============================================================
//
// �J�����̓��� [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"

//=============================================================
// [CCameraMove] �X�V
//=============================================================
void CCameraMove::Update()
{
	// �^�[�Q�b�g�����݂��邩�m�F����
	if (m_pTarget == nullptr)
	{ // �^�[�Q�b�g�����݂��Ȃ��Ƃ�
		return;
	}

	Transform* pTargetTrans = m_pTarget->transform;

	// ��]
	transform->SetRot(/*pTargetTrans->GetRotX() * 0.8f +*/ 0.3f, pTargetTrans->GetWRotY(), 0.0f);

	// �J�����̖ڕW�n�_���v�Z����
	D3DXVECTOR3 objectivePos;
	objectivePos = pTargetTrans->GetWPos();
	//objectivePos += {
	//	0.0f,
	//	40.0f,
	//	0.0f
	//};
	objectivePos += {
		sinf(pTargetTrans->GetWRotY()) * -150.0f,
		100.0f,
		cosf(pTargetTrans->GetWRotY()) * -150.0f
	};
	//objectivePos -= {
	//		sinf(pTargetTrans->GetWRotY() + D3DX_PI * 0.5f) * pTargetTrans->GetRotZ() * 20.0f,
	//		fabsf(pTargetTrans->GetRotZ()) * 10.0f,
	//		cosf(pTargetTrans->GetWRotY() + D3DX_PI * 0.5f) * pTargetTrans->GetRotZ() * 20.0f
	//};

	// �ڕW�n�_�Ɉړ�����
	transform->SetPos(objectivePos);
}

//=============================================================
// [CCameraMove] �I�u�W�F�N�g���j�����ꂽ�Ƃ��̏���
//=============================================================
void CCameraMove::OnDestroyOtherObject(GameObject* other)
{
	if (other == m_pTarget)
	{ // �^�[�Q�b�g�̂Ƃ�
		m_pTarget = nullptr;
	}
}