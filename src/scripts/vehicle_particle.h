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
#include "terrain.h"
#include "lake.h"
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
	//@brief 軌道の更新
	void UpdateTrajectory();

	//@brief 後方パーティクルの更新
	void UpdateBackParticle();

	CVehicle* m_vehicle;				// バイク
	Terrain* m_terrain;					// 地形
	LakeManager* m_lake;			// 池
	GameObject* m_trajectory;	// 軌道
	float m_backParticleTimer;

	// 定数
	const float BP_MIN_GENERATE = 15.0f;		// 最小生成
	const float BP_RANDOM_TIME = 0.1f;		// 生成時間
	const float BP_RANDOM_WIDTH = 2.0f;	// 横幅
	const float BP_BACK_RANGE = 10.0f;		// 後方範囲
	const float BP_MIN_SIZE = 0.5f;					// パーティクルの最小サイズ
	const float BP_SPEED_RATE = 0.01f;			// 速さによるレート
	const float BP_EXPANSE_VALUE = 5.0f;		// 広がり
	const float BP_GRAVITY = -80.0f;				// 重力
	const float BG_LAKE_SPACE = 20.0f;			// 池範囲の余分
};

#endif // !_VEHICLE_PARTICLE_H_
