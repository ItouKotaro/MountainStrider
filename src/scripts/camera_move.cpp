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

	// �J�����̖ڕW�n�_���v�Z����
	D3DXVECTOR3 objectivePos;
	objectivePos = pTargetTrans->GetWPos();
	objectivePos += {
		sinf(pTargetTrans->GetWRotY()) * -20.0f,
		40.0f,
		cosf(pTargetTrans->GetWRotY()) * -20.0f
	};
	objectivePos -= {
			sinf(pTargetTrans->GetWRotY() + D3DX_PI * 0.5f) * pTargetTrans->GetRotZ() * 20.0f,
			fabsf(pTargetTrans->GetRotZ()) * 10.0f,
			cosf(pTargetTrans->GetWRotY() + D3DX_PI * 0.5f) * pTargetTrans->GetRotZ() * 20.0f
	};

	// �ڕW�n�_�Ɉړ�����
	transform->SetPos(objectivePos);

	
	// ��]
	transform->SetRot(0.15f, pTargetTrans->GetWRotY(), 0.0f);
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