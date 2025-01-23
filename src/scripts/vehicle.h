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
#include "scripts/status_ui.h"
#include "scripts/pause.h"

// �ԗ�
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

	// �f�[�^�擾
	float GetSpeed() { return m_fSpeed; }
	float GetFuel() { return m_fuel; }
	float GetEndurance() { return m_endurance; }
	float GetGroundDistance() { return m_groundDistance; }
	bool GetFlying() { return m_flying; }
	float GetStartFlyingAngle() { return m_startFlyingAngle; }

	static const float ENGINEFORCE_VALUE;			// �G���W���͂̒l
	static const float MIN_ENGINEFORCE_VALUE;	// �G���W���͂̍ŏ��i�A�N�Z���𓥂�ł��Ȃ��Ƃ��j
	static const float STEERING_VALUE;				// �X�e�A�����O�̒l

	static const float MAX_ENGINEFORCE;			// �G���W���͂̍ő�l
	static const float MAX_STEERING;				// �X�e�A�����O�̍ő�l
	static const float MAX_FUEL;						// �R���̍ő�l
	static const float MAX_ENDURANCE;			// �ϋv�l�̍ő�l

	static const float GRAVITY;							// �d��

	static const float FLYING_DISTANCE;			// ���ł���Ɣ��f���鋗��
	static const float GROUND_DISTANCE;			// �n�ʂƔ��f���鋗��
	static const int VIBRATION_VALUE;				// �o�C�u���[�V�����̋���

private:
	void LandingControlVehicle();		// �n�ʎ��̑���
	void FlyingControlVehicle();		// �n�ʎ��̑���
	void UpdateSpeedMeter();			// �X�s�[�h�̍X�V
	void UpdateStatusUI();				// �X�e�[�^�XUI�̍X�V
	void UpdateGroundDistance();		// �n�ʋ����̍X�V

	CCollision* m_limitField;
	Pause* m_pause;
	GameObject* m_pPlayer;
	GameObject* m_pHandle;		// �n���h��
	GameObject* m_pFrontTire;	// �O��
	GameObject* m_pBackTire;	// ���

	static float m_fuel;							// �R��
	static float m_endurance;				// �ϋv�l
	D3DXVECTOR3 m_flyingPosture;		// ��s���̎p��
	float m_fuelConsumption;				// ����R��

	// �n�ʏ��
	bool m_flying;						// ���ł���
	float m_groundDistance;			// �n�ʂƂ̋���
	float m_startFlyingAngle;					// ��񂾎���Y�p�x

	// �X�e�[�^�XUI
	CStatusUI* m_pStatusUI;

	// ���x
	float m_fSpeed;							// ���x
	int m_measureCounter;				// �v���J�E���^�[
	D3DXVECTOR3 m_measurePos;	// �v���ʒu
};

#endif // !_VEHICLE_H_
