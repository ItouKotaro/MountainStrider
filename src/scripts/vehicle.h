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

// レイキャスト車両
class CRaycastVehicle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	void UpdateRay();

	btRaycastVehicle* m_vehicle;
	btDefaultVehicleRaycaster* m_raycaster;
	btRaycastVehicle::btVehicleTuning m_tuning;
	btRigidBody* m_body;
	btMotionState* m_motionstate;

	// モデル
	GameObject* m_pFrontWheel;
	GameObject* m_pBackWheel;
	GameObject* m_pText;
};

class CCar : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	static const float ENGINEFORCE_VALUE;		// エンジン力の値
	static const float STEERING_VALUE;			// ステアリングの値
private:
	GameObject* m_pFrontLeftTire;
	GameObject* m_pFrontRightTire;
	GameObject* m_pBackLeftTire;
	GameObject* m_pBackRightTire;

	float m_fEngineForce;
	float m_fSteering;
};

#endif // !_VEHICLE_H_
