//=============================================================
//
// �ԗ� [vehicle.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _VEHICLE_H_
#define _VEHICLE_H_

#include "component.h"
#include "internal/physics.h"
#include "BulletDynamics/Vehicle/btRaycastVehicle.h"

// �ԗ�
class CVehicle : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	static const float ENGINEFORCE_VALUE;		// �G���W���͂̒l
	static const float STEERING_VALUE;			// �X�e�A�����O�̒l
	static const float MAX_ENGINEFORCE;			// �G���W���͂̍ő�l
	static const float MAX_STEERING;				// �X�e�A�����O�̍ő�l
private:
	void UpdateSpeedMeter();
	GameObject* m_pHandle;
	GameObject* m_pFrontTire;
	GameObject* m_pBackTire;

	float m_fEngineForce;
	float m_fSteering;

	// ���x
	GameObject* m_pSpeedText;
	float m_fSpeed;
	int m_measureCounter;
	D3DXVECTOR3 m_measurePos;
};

#endif // !_VEHICLE_H_
