//=============================================================
//
// ��Ԉړ��R���|�[�l���g [movement_space.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MOVEMENT_SPACE_H_
#define _MOVEMENT_SPACE_H_

#include <component.h>

// ��Ԉړ��R���|�[�l���g�N���X�̒�`
class CMovementSpace : public Component
{
public:
	CMovementSpace(float fSpeed = 1.0f, float fMouseSpeed = 0.008f);
	void Update();
private:
	D3DXVECTOR2 GetLeftClickingMouseMove();		// ���N���b�N���̃}�E�X�ړ��ʂ̎擾
	D3DXVECTOR2 GetRightClickingMouseMove();		// �E�N���b�N���̃}�E�X�ړ��ʂ̎擾
	float GetMoveAngle();

	float m_fSpeed;
	float m_fMouseSpeed;
	POINT m_ptBeforeLMouse;
	POINT m_ptBeforeRMouse;
};

#endif // !_MOVEMENT_SPACE_H_
