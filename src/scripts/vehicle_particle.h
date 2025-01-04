//=============================================================
//
// �ԗ��p�[�e�B�N�� [vehicle_particle.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_PARTICLE_H_
#define _VEHICLE_PARTICLE_H_

#include "component.h"
#include "vehicle.h"
#include "component/3d/particle.h"

// �ԗ��̃p�[�e�B�N���Ǘ�
class VehicleParticle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	void UpdateTrajectory();

	CVehicle* m_vehicle;
	SingleComponent<ParticleSystem>* m_particleSystem;
	GameObject* m_trajectory;
};

#endif // !_VEHICLE_PARTICLE_H_
