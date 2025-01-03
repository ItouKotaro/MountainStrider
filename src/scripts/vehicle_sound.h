//=============================================================
//
// 車両のサウンド [vehicle_sound.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_SOUND_H_
#define _VEHICLE_SOUND_H_

#include "component.h"
#include "vehicle.h"
#include "component/other/audio.h"

// 車両のサウンド
class VehicleSound : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	CVehicle* m_vehicle;				// バイク
	GameObject* m_soundObj;

	AudioEventInstance m_vehicleSound;		// 走行音
	float m_vehicleRPM;
	float m_vehicleRPMLimit;
};

#endif // !_VEHICLE_SOUND_H_
