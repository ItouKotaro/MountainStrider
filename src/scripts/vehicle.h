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
	GameObject* m_pHandle;
	GameObject* m_pFrontTire;
	GameObject* m_pBackTire;

	float m_fEngineForce;
	float m_fSteering;

	// 速度
	GameObject* m_pSpeedText;
	float m_fSpeed;
	int m_measureCounter;
	D3DXVECTOR3 m_measurePos;
};

#endif // !_VEHICLE_H_
