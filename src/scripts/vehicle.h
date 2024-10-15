//=============================================================
//
// �ԗ� [vehicle.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "component.h"

// �ԗ�
class CVehicle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:

};

#endif // !_VEHICLE_H_
