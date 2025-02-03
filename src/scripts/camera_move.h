//=============================================================
//
// カメラの動き [camera_move.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _CAMERA_MOVE_H_
#define _CAMERA_MOVE_H_

#include "component.h"
#include "manager.h"

class CCameraMove : public Component
{
public:
	void Init() override;
	void Update() override;
	void OnDestroyOtherObject(GameObject* other) override;

	// ターゲットを設定する
	void SetTarget(GameObject* pTarget) { m_pTarget = pTarget; }
private:
	void UpdateFPS();
	void UpdateTPS();

	GameObject* m_pTarget;								// ターゲット
	D3DXVECTOR3 m_cameraRot;						// 回転
	D3DXVECTOR3 m_posS;
	int m_moveCounter;
	CManager::CursorPos m_oldCursor;					// 前回のカーソル位置
	
	const float UP_LIMIT = 0.5f;							// 上向きの制限
	const float BOTTOM_LIMIT = 1.0f;					// 下向きの制限
};

#endif // !_CAMERA_MOVE_H_
