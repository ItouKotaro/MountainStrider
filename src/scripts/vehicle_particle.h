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

	CVehicle* m_vehicle;				// �o�C�N
	Terrain* m_terrain;					// �n�`
	LakeManager* m_lake;			// �r
	GameObject* m_trajectory;	// �O��
	float m_backParticleTimer;

	// �萔
	const float BP_MIN_GENERATE = 15.0f;		// �ŏ�����
	const float BP_RANDOM_TIME = 0.1f;		// ��������
	const float BP_RANDOM_WIDTH = 2.0f;	// ����
	const float BP_BACK_RANGE = 10.0f;		// ����͈�
	const float BP_MIN_SIZE = 0.5f;					// �p�[�e�B�N���̍ŏ��T�C�Y
	const float BP_SPEED_RATE = 0.01f;			// �����ɂ�郌�[�g
	const float BP_EXPANSE_VALUE = 5.0f;		// �L����
	const float BP_GRAVITY = -80.0f;				// �d��
	const float BG_LAKE_SPACE = 20.0f;			// �r�͈̗͂]��
};

#endif // !_VEHICLE_PARTICLE_H_
