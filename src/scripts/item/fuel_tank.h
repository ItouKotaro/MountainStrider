//=============================================================
//
// �R���^���N [fuel_tank.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _FUEL_TANK_H_
#define _FUEL_TANK_H_

#include "component.h"

// �R���^���N
class FuelTank : public Component
{
public:
	void Init() override;
	void OnTriggerEnter(GameObject* other) override;
	static const float FUEL_VALUE;
private:
	GameObject* m_pVehicle;
};

#endif // !_FUEL_TANK_H_
