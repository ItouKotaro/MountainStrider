//=============================================================
//
// ��Ԉړ��R���|�[�l���g [movement_space.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "movement_space.h"

//=============================================================
// [CMovementSpace] �R���X�g���N�^
//=============================================================
CMovementSpace::CMovementSpace(float fSpeed, float fMouseSpeed)
{
	m_ptBeforeLMouse = {0, 0};
	m_ptBeforeRMouse = { 0, 0 };
	m_fSpeed = fSpeed;
	m_fMouseSpeed = fMouseSpeed;
}

//=============================================================
// [CMovementSpace] �X�V
//=============================================================
void CMovementSpace::Update()
{
	D3DXVECTOR2 LeftClickMove = GetLeftClickingMouseMove();
	D3DXVECTOR2 RightClickMove = GetRightClickingMouseMove();

	// �ړ�����
	if (INPUT_INSTANCE->onPress("w") ||
		INPUT_INSTANCE->onPress("a") ||
		INPUT_INSTANCE->onPress("s") ||
		INPUT_INSTANCE->onPress("d"))
	{
		float fAngle = -GetMoveAngle() + transform->GetRotY() + D3DX_PI * 0.5f;  // �p�x�擾
		transform->Translate(sinf(fAngle) * m_fSpeed, 0.0f, cosf(fAngle) * m_fSpeed);
	}

	//--------------------------------------------------------
	// �ʒu��ύX�i���E�N���b�N�j
	if (GetKeyState(VK_LBUTTON) & 0x80 && GetKeyState(VK_RBUTTON) & 0x80)
	{
		transform->Translate(
			cosf(transform->GetRotY()) * LeftClickMove.y * m_fSpeed,
			0.0f,
			sinf(transform->GetRotY()) * LeftClickMove.y * m_fSpeed
		);

		transform->Translate(
			cosf(D3DX_PI / 2 + transform->GetRotY()) * LeftClickMove.x * m_fSpeed,
			0.0f,
			sinf(D3DX_PI / 2 + transform->GetRotY()) * LeftClickMove.x * m_fSpeed
		);

		return;
	}

	//--------------------------------------------------------
	// ���_�̈ʒu��ύX�i���N���b�N�j
	transform->Rotate(LeftClickMove.y * m_fMouseSpeed, -LeftClickMove.x * m_fMouseSpeed, 0.0f);
}

//=============================================================
// [CMovementSpace] ���N���b�N���̃}�E�X�ړ��ʂ̎擾
//=============================================================
D3DXVECTOR2 CMovementSpace::GetLeftClickingMouseMove()
{
	// �}�E�X�ړ��ʗp�̕ϐ�
	D3DXVECTOR2 mouseMove = D3DXVECTOR2(0.0f, 0.0f);

	if (GetKeyState(VK_LBUTTON) & 0x80 && GetActiveWindow() != nullptr)
	{ // ���N���b�N��
		// �}�E�X�̍��W���擾����
		POINT point;
		GetCursorPos(&point);

		// �O��̃}�E�X���W�Ɣ�r���A�ړ��ʂ��擾����
		mouseMove.x = (float)(m_ptBeforeLMouse.x - point.x);
		mouseMove.y = (float)(m_ptBeforeLMouse.y - point.y);

		// ����̃}�E�X���W��ۑ�����
		GetCursorPos(&m_ptBeforeLMouse);
	}
	else
	{ // ���N���b�N����Ă��Ȃ��Ƃ�
		// ���݂̃}�E�X�ʒu��ۑ�
		GetCursorPos(&m_ptBeforeLMouse);
	}

	return mouseMove;	// �ړ��ʂ�Ԃ�
}

//=============================================================
// [CMovementSpace] �E�N���b�N���̃}�E�X�ړ��ʂ̎擾
//=============================================================
D3DXVECTOR2 CMovementSpace::GetRightClickingMouseMove()
{
	// �}�E�X�ړ��ʗp�̕ϐ�
	D3DXVECTOR2 mouseMove = D3DXVECTOR2(0.0f, 0.0f);

	if (GetKeyState(VK_RBUTTON) & 0x80 && GetActiveWindow() != nullptr)
	{ // �E�N���b�N��
		// �}�E�X�̍��W���擾����
		POINT point;
		GetCursorPos(&point);

		// �O��̃}�E�X���W�Ɣ�r���A�ړ��ʂ��擾����
		mouseMove.x = (float)(m_ptBeforeRMouse.x - point.x);
		mouseMove.y = (float)(m_ptBeforeRMouse.y - point.y);

		// ����̃}�E�X���W��ۑ�����
		GetCursorPos(&m_ptBeforeRMouse);
	}
	else
	{ // �E�N���b�N����Ă��Ȃ��Ƃ�
		// ���݂̃}�E�X�ʒu��ۑ�
		GetCursorPos(&m_ptBeforeRMouse);
	}

	return mouseMove;	// �ړ��ʂ�Ԃ�
}

//=============================================================
// [CMovementSpace] �ړ��p�x���擾
//=============================================================
float CMovementSpace::GetMoveAngle()
{
	float fMoveX = 0.0f;
	float fMoveY = 0.0f;

	if (INPUT_INSTANCE->onPress("w"))
	{
		fMoveY += 1.0f;
	}
	if (INPUT_INSTANCE->onPress("s"))
	{
		fMoveY += -1.0f;
	}
	if (INPUT_INSTANCE->onPress("a"))
	{
		fMoveX += -1.0f;
	}
	if (INPUT_INSTANCE->onPress("d"))
	{
		fMoveX += 1.0f;
	}

	return atan2f(fMoveY, fMoveX);
}