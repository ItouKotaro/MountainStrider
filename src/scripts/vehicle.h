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

	// データ取得
	float GetSpeed() { return m_fSpeed; }
	float GetFuel() { return m_fuel; }
	float GetEndurance() { return m_endurance; }

	static const float ENGINEFORCE_VALUE;			// エンジン力の値
	static const float MIN_ENGINEFORCE_VALUE;	// エンジン力の最小（アクセルを踏んでいないとき）
	static const float STEERING_VALUE;				// ステアリングの値

	static const float MAX_ENGINEFORCE;			// エンジン力の最大値
	static const float MAX_STEERING;				// ステアリングの最大値
	static const float MAX_FUEL;						// 燃料の最大値
	static const float MAX_ENDURANCE;			// 耐久値の最大値
private:
	void ControlVehicle();				// 操作
	void UpdateSpeedMeter();		// スピードの更新
	void UpdateStatusUI();			// ステータスUIの更新

	GameObject* m_pHandle;		// ハンドル
	GameObject* m_pFrontTire;	// 前輪
	GameObject* m_pBackTire;	// 後輪

	float m_fuel;						// 燃料
	float m_endurance;				// 耐久値

	// ステータスUI
	GameObject* m_pStatusUI;

	// 速度
	float m_fSpeed;							// 速度
	int m_measureCounter;				// 計測カウンター
	D3DXVECTOR3 m_measurePos;	// 計測位置
};

#endif // !_VEHICLE_H_
