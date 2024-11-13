//=============================================================
//
// 車両 [vehicle.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "component.h"
#include "internal/physics.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

// 車両
class CVehicle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	static const float ENGINEFORCE_VALUE;		// エンジン力の値
	static const float STEERING_VALUE;			// ステアリングの値
	static const float MAX_ENGINEFORCE;			// エンジン力の最大値
	static const float MAX_STEERING;				// ステアリングの最大値
private:
	void UpdateSpeedMeter();
	GameObject* m_pHandle;		// ハンドル
	GameObject* m_pFrontTire;	// 前輪
	GameObject* m_pBackTire;	// 後輪

	float m_fSteering;					// ステアリング

	// ステータスUI
	GameObject* m_pStatusUI;

	// 速度
	GameObject* m_pSpeedText;		// 状態テキスト
	float m_fSpeed;							// 速度
	int m_measureCounter;				// 計測カウンター
	D3DXVECTOR3 m_measurePos;	// 計測位置
};

#endif // !_VEHICLE_H_
