//=============================================================
//
// 車両パーティクル [vehicle_particle.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_PARTICLE_H_
#define _VEHICLE_PARTICLE_H_

#include "component.h"
#include "vehicle.h"
#include "component/3d/particle.h"

// 車両のパーティクル管理
class VehicleParticle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;
private:
	CVehicle* m_vehicle;
	SingleComponent<ParticleSystem>* m_particleSystem;
};

#endif // !_VEHICLE_PARTICLE_H_
