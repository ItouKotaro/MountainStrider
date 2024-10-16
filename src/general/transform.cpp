//=============================================================
//
// �g�����X�t�H�[�� [transform.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "transform.h"
#include "internal/physics.h"

//=============================================================
// [Transform] �ʒu�̐ݒ�
//=============================================================
void Transform::SetPos(float x, float y, float z)
{
	m_position = { x, y, z };
}

//=============================================================
// [Transform] ��]�̐ݒ�
//=============================================================
void Transform::SetRot(float x, float y, float z)
{
	m_rotation = { x, y, z };
}

//=============================================================
// [Transform] �e��ݒ肷��
//=============================================================
void Transform::SetParent(Transform* pParent)
{
	// nullptr���ݒ肳�ꂽ�Ƃ�
	if (pParent == nullptr)
	{
		m_pParent = nullptr;
		return;
	}

	// �����Ɏ����̃|�C���^�������ꂽ�Ƃ�
	if (pParent == this)
		return;

	// �w�肵���g�����X�t�H�[���̐e�������ȊO�̎��̂݁A�ݒ肷��
	if (pParent->GetParent() != this)
		m_pParent = pParent;
}

//=============================================================
// [Transform] ���[���h���W�ł̈ʒu���擾
//=============================================================
D3DXVECTOR3 Transform::GetWPos()
{
	D3DXMATRIX mtx = GetMatrix();
	D3DXVECTOR3 pos = { mtx._41, mtx._42, mtx._43 };
	return pos;
}

//=============================================================
// [Transform] ���[���h���W�ł̉�]���擾
//=============================================================
D3DXVECTOR3 Transform::GetWRot()
{
	//rot.z = atan2f(mtx._12, mtx._22);
	//rot.x = asinf(-mtx._32);
	//rot.y = atan2f(mtx._31, mtx._33);

	//if (fabsf(cosf(rot.x)) < 1.0e-6f)
	//{
	//	rot.z += mtx._12 > 0.0f ? D3DX_PI : -D3DX_PI;
	//	rot.y += mtx._31 > 0.0f ? D3DX_PI : -D3DX_PI;
	//}

	// �e�̃��[���h���W�����[�J�����W�ɑ���
	D3DXVECTOR3 worldRot = { 0.0f, 0.0f, 0.0f };
	Transform* worldTransform = this;
	do
	{
		// ���W�𑫂�
		worldRot += worldTransform->GetRot();

		// ���̐e�ɐi��
		worldTransform = worldTransform->GetParent();

	} while (worldTransform != nullptr);

	// ���K��
	worldRot = NormalizeRotation(worldRot);

	return worldRot;
}

//=============================================================
// [Transform] ���[���h���W�ł̃X�P�[�����擾
//=============================================================
D3DXVECTOR3 Transform::GetWScale()
{
	// �e�̃��[���h���W�����[�J�����W�Ɋ|����
	D3DXVECTOR3 worldScale = { 1.0f, 1.0f, 1.0f };
	Transform* worldTransform = this;
	do
	{
		// ���W���|����
		worldScale.x *= worldTransform->GetScale().x;
		worldScale.y *= worldTransform->GetScale().y;
		worldScale.z *= worldTransform->GetScale().z;

		// ���̐e�ɐi��
		worldTransform = worldTransform->GetParent();

	} while (worldTransform != nullptr);

	return worldScale;
}

//=============================================================
// [Transform] �}�g���b�N�X���擾����
//=============================================================
D3DXMATRIX& Transform::GetMatrix()
{
	// �ϐ�
	D3DXMATRIX mtxScale, mtxRot, mtxTrans;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtx);

	// �X�P�[���𔽉f
	D3DXVECTOR3 scale = GetScale();
	D3DXMatrixScaling(&mtxScale, scale.x, scale.y, scale.z);
	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxScale);

	// �����𔽉f
	D3DXVECTOR3 rotation = GetRot();
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rotation.y, rotation.x, rotation.z);
	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxRot);

	// �ʒu�𔽉f
	D3DXVECTOR3 position = GetPos();
	D3DXMatrixTranslation(&mtxTrans, position.x, position.y, position.z);
	D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxTrans);

	if (GetParent() != nullptr)
	{
		D3DXMATRIX mtxParent;
		mtxParent = GetParent()->GetMatrix();
		D3DXMatrixMultiply(&m_mtx, &m_mtx, &mtxParent);
	}

	return m_mtx;
}

//=============================================================
// [Transform] ���Z�q ==
//=============================================================
bool Transform::operator ==(Transform& transform)
{
	if (GetWPos() == transform.GetWPos() &&
		GetWRot() == transform.GetWRot() &&
		GetWScale() == transform.GetWScale() &&
		m_pParent == transform.m_pParent)
	{
		return true;
	}
	return false;
}

//=============================================================
// [Transform] �w��ʒu�̕���������
//=============================================================
void Transform::LookAt(D3DXVECTOR3 target)
{
	// ���݂̈ʒu���擾����
	D3DXVECTOR3 pos = GetWPos();

	// �����ׂ��������Z�o����
	D3DXVECTOR3 rot;
	float fTargetAngle = -atan2f(target.x - pos.x, target.z - pos.z);
	float fTargetHeightAngle = atan2f(sqrtf(fabsf(target.x - pos.x) * fabsf(target.x - pos.x) +
	fabsf(target.z - pos.z) * fabsf(target.z - pos.z)), (target.y - pos.y));

	// ��������
	rot = D3DXVECTOR3(-fTargetHeightAngle + D3DX_PI * 0.5f, fTargetAngle, 0.0f);
	SetRot(rot);
}

//=============================================================
// [Transform] ��]�̐��K��
//=============================================================
D3DXVECTOR3 Transform::NormalizeRotation(const D3DXVECTOR3& rot)
{
	D3DXVECTOR3 normalized = rot;
	if (normalized.x > D3DX_PI)
	{
		normalized.x -= D3DX_PI * 2;
	}
	if (normalized.y > D3DX_PI)
	{
		normalized.y -= D3DX_PI * 2;
	}
	if (normalized.z > D3DX_PI)
	{
		normalized.z -= D3DX_PI * 2;
	}

	if (normalized.x < -D3DX_PI)
	{
		normalized.x += D3DX_PI * 2;
	}
	if (normalized.y < -D3DX_PI)
	{
		normalized.y += D3DX_PI * 2;
	}
	if (normalized.z < -D3DX_PI)
	{
		normalized.z += D3DX_PI * 2;
	}
	return normalized;
}