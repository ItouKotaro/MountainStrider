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
private:
	GameObject* m_pHandle;
	GameObject* m_pFrontTire;
	GameObject* m_pBackTire;

	float m_fEngineForce;
	float m_fSteering;
};

// レイキャスト車両
class CRaycastVehicle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	btRaycastVehicle* m_vehicle;
	btDefaultVehicleRaycaster* m_raycaster;
	btRaycastVehicle::btVehicleTuning m_tuning;
	btRigidBody* m_body;
	btMotionState* m_motionstate;
	GameObject* m_pText;
};

#endif // !_VEHICLE_H_
