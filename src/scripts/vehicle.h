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

// ŽÔ—¼
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
};

#endif // !_VEHICLE_H_
