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
#include "terrain.h"
#include "lake.h"
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
	//@brief �O���̍X�V
	void UpdateTrajectory();

	//@brief ����p�[�e�B�N���̍X�V
	void UpdateBackParticle();

	CVehicle* m_vehicle;
	Terrain* m_terrain;
	LakeManager* m_lake;
	GameObject* m_trajectory;
	float m_backParticleTimer;

	// �萔
	const float BP_MIN_GENERATE = 15.0f;
	const float BP_RANDOM_TIME = 0.1f;
	const float BP_RANDOM_WIDTH = 2.0f;
	const float BP_BACK_RANGE = 10.0f;
	const float BP_MIN_SIZE = 0.5f;
	const float BP_SPEED_RATE = 0.01f;
	const float BP_EXPANSE_VALUE = 5.0f;
	const float BP_GRAVITY = -80.0f;
	const float BG_LAKE_SPACE = 20.0f;
};

#endif // !_VEHICLE_PARTICLE_H_
