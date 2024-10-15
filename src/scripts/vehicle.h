//=============================================================
//
// ŽÔ—¼ [vehicle.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "component.h"
#include "internal/physics.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

// ŽÔ—¼
class CVehicle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	btRigidBody* m_bodyRigidbody;
	btMotionState* m_pMotionState;

	btRaycastVehicle::btVehicleTuning m_vehicleTuning;
	btDefaultVehicleRaycaster* m_vehicleRaycaster;
	btRaycastVehicle* m_raycastVehicle;
};

#endif // !_VEHICLE_H_
