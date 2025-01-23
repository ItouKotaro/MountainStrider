//=============================================================
//
// 車両 [vehicle.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "component.h"
#include "internal/physics.h"
#include "scripts/status_ui.h"
#include "scripts/pause.h"

// 車両
class CVehicle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void SetPos(const D3DXVECTOR3& pos);
	void AddDamage(const float& value);
	void AddFuel(const float& value);
	void AddEndurance(const float& value);
	void SetStatusUI(CStatusUI* pStatusUI) { m_pStatusUI = pStatusUI; }
	float GetFuelConsumption() { return m_fuelConsumption; }

	static void ResetState()
	{
		m_fuel = MAX_FUEL;
		m_endurance = MAX_ENDURANCE;
	}

	// データ取得
	float GetSpeed() { return m_fSpeed; }
	float GetFuel() { return m_fuel; }
	float GetEndurance() { return m_endurance; }
	float GetGroundDistance() { return m_groundDistance; }
	bool GetFlying() { return m_flying; }
	float GetStartFlyingAngle() { return m_startFlyingAngle; }

	static const float ENGINEFORCE_VALUE;			// エンジン力の値
	static const float MIN_ENGINEFORCE_VALUE;	// エンジン力の最小（アクセルを踏んでいないとき）
	static const float STEERING_VALUE;				// ステアリングの値

	static const float MAX_ENGINEFORCE;			// エンジン力の最大値
	static const float MAX_STEERING;				// ステアリングの最大値
	static const float MAX_FUEL;						// 燃料の最大値
	static const float MAX_ENDURANCE;			// 耐久値の最大値

	static const float GRAVITY;							// 重力

	static const float FLYING_DISTANCE;			// 飛んでいると判断する距離
	static const float GROUND_DISTANCE;			// 地面と判断する距離
	static const int VIBRATION_VALUE;				// バイブレーションの強さ

private:
	void LandingControlVehicle();		// 地面時の操作
	void FlyingControlVehicle();		// 地面時の操作
	void UpdateSpeedMeter();			// スピードの更新
	void UpdateStatusUI();				// ステータスUIの更新
	void UpdateGroundDistance();		// 地面距離の更新

	CCollision* m_limitField;
	Pause* m_pause;
	GameObject* m_pPlayer;
	GameObject* m_pHandle;		// ハンドル
	GameObject* m_pFrontTire;	// 前輪
	GameObject* m_pBackTire;	// 後輪

	static float m_fuel;							// 燃料
	static float m_endurance;				// 耐久値
	D3DXVECTOR3 m_flyingPosture;		// 飛行時の姿勢
	float m_fuelConsumption;				// 消費燃料

	// 地面情報
	bool m_flying;						// 飛んでいる
	float m_groundDistance;			// 地面との距離
	float m_startFlyingAngle;					// 飛んだ時のY角度

	// ステータスUI
	CStatusUI* m_pStatusUI;

	// 速度
	float m_fSpeed;							// 速度
	int m_measureCounter;				// 計測カウンター
	D3DXVECTOR3 m_measurePos;	// 計測位置
};

#endif // !_VEHICLE_H_
