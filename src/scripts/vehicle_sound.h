//=============================================================
//
// �ԗ��̃T�E���h [vehicle_sound.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_SOUND_H_
#define _VEHICLE_SOUND_H_

#include "component.h"
#include "vehicle.h"
#include "component/other/sound.h"

// �ԗ��̃T�E���h
class VehicleSound : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	CVehicle* m_vehicle;				// �o�C�N

	CSound* m_runningSE;			// ���s��
	float m_runnningVolume;
	float m_runnningPitch;
	float m_runnningPitchLimit;
	float m_runnningVolumeLimit;
};

#endif // !_VEHICLE_SOUND_H_
