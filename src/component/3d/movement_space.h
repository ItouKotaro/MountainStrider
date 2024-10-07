//=============================================================
//
// 空間移動コンポーネント [movement_space.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _MOVEMENT_SPACE_H_
#define _MOVEMENT_SPACE_H_

#include <component.h>

// 空間移動コンポーネントクラスの定義
class CMovementSpace : public Component
{
public:
	CMovementSpace(float fSpeed = 1.0f, float fMouseSpeed = 0.008f);
	void Update();
private:
	D3DXVECTOR2 GetLeftClickingMouseMove();		// 左クリック時のマウス移動量の取得
	D3DXVECTOR2 GetRightClickingMouseMove();		// 右クリック時のマウス移動量の取得
	float GetMoveAngle();

	float m_fSpeed;
	float m_fMouseSpeed;
	POINT m_ptBeforeLMouse;
	POINT m_ptBeforeRMouse;
};

#endif // !_MOVEMENT_SPACE_H_
