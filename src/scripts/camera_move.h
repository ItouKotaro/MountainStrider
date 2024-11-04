//=============================================================
//
// �J�����̓��� [camera_move.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _CAMERA_MOVE_H_
#define _CAMERA_MOVE_H_

#include "component.h"

class CCameraMove : public Component
{
public:
	void Update() override;
	void OnDestroyOtherObject(GameObject* other) override;

	// �^�[�Q�b�g��ݒ肷��
	void SetTarget(GameObject* pTarget) { m_pTarget = pTarget; }
private:
	GameObject* m_pTarget;	// �^�[�Q�b�g
};

#endif // !_CAMERA_MOVE_H_
