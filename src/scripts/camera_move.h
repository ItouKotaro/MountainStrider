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
	void Init() override;
	void Update() override;
	void OnDestroyOtherObject(GameObject* other) override;

	// �^�[�Q�b�g��ݒ肷��
	void SetTarget(GameObject* pTarget) { m_pTarget = pTarget; }
private:
	GameObject* m_pTarget;			// �^�[�Q�b�g
	D3DXVECTOR3 m_cameraRot;	// ��]
};

#endif // !_CAMERA_MOVE_H_
