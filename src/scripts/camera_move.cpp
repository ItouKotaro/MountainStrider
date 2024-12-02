//=============================================================
//
// �J�����̓��� [camera_move.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera_move.h"
#include "scripts/vehicle.h"

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

	// ���̎擾
	Transform* pTargetTrans = m_pTarget->transform;
	D3DXQUATERNION objectiveRot = pTargetTrans->GetQuaternion();
	D3DXQUATERNION targetRot = transform->GetQuaternion();
	D3DXVECTOR3 objectivePos;

	bool flying = m_pTarget->GetComponent<CVehicle>()->GetFlying();
	if (!flying)
	{
		// �J�����̖ڕW�n�_���v�Z����
		D3DXMATRIX vehicleMtx = pTargetTrans->GetMatrix();
		objectivePos += {
				0.0f,
				40.0f,
				-150.0f
		};
		D3DXVec3TransformCoord(&objectivePos, &objectivePos, &vehicleMtx);
	}
	else
	{
		// �J�����̖ڕW�n�_���v�Z����
		objectivePos += {
				0.0f,
				30.0f,
				-250.0f
		};
		D3DXMATRIX mtx;
		D3DXMATRIX mtxPos = pTargetTrans->GetTranslationMatrix();
		D3DXMatrixRotationY(&mtx, m_pTarget->GetComponent<CVehicle>()->GetStartFlyingAngle());
		D3DXMatrixMultiply(&mtx, &mtx, &mtxPos);
		D3DXVec3TransformCoord(&objectivePos, &objectivePos, &mtx);
	}

	// ��]
	objectiveRot = Benlib::LookAt(transform->GetWPos(), pTargetTrans->GetWPos() + D3DXVECTOR3(0.0f, 50.0f, 0.0f));
	D3DXQuaternionSlerp(&objectiveRot, &targetRot, &objectiveRot, 0.08f);
	transform->SetQuaternion(objectiveRot);

	// �ڕW�n�_�Ɉړ�����
	transform->SetPos(transform->GetWPos()+(objectivePos - transform->GetWPos()) * 0.08f);
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