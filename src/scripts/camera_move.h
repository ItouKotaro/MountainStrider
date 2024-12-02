//=============================================================
//
// カメラの動き [camera_move.h]
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

	// ターゲットを設定する
	void SetTarget(GameObject* pTarget) { m_pTarget = pTarget; }
private:
	GameObject* m_pTarget;			// ターゲット
	D3DXVECTOR3 m_cameraRot;	// 回転
};

#endif // !_CAMERA_MOVE_H_
