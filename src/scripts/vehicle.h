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

	// �f�[�^�擾
	float GetSpeed() { return m_fSpeed; }
	float GetFuel() { return m_fuel; }
	float GetEndurance() { return m_endurance; }

	static const float ENGINEFORCE_VALUE;			// �G���W���͂̒l
	static const float MIN_ENGINEFORCE_VALUE;	// �G���W���͂̍ŏ��i�A�N�Z���𓥂�ł��Ȃ��Ƃ��j
	static const float STEERING_VALUE;				// �X�e�A�����O�̒l

	static const float MAX_ENGINEFORCE;			// �G���W���͂̍ő�l
	static const float MAX_STEERING;				// �X�e�A�����O�̍ő�l
	static const float MAX_FUEL;						// �R���̍ő�l
	static const float MAX_ENDURANCE;			// �ϋv�l�̍ő�l
private:
	void ControlVehicle();				// ����
	void UpdateSpeedMeter();		// �X�s�[�h�̍X�V
	void UpdateStatusUI();			// �X�e�[�^�XUI�̍X�V

	GameObject* m_pHandle;		// �n���h��
	GameObject* m_pFrontTire;	// �O��
	GameObject* m_pBackTire;	// ���

	float m_fuel;						// �R��
	float m_endurance;				// �ϋv�l

	// �X�e�[�^�XUI
	GameObject* m_pStatusUI;

	// ���x
	float m_fSpeed;							// ���x
	int m_measureCounter;				// �v���J�E���^�[
	D3DXVECTOR3 m_measurePos;	// �v���ʒu
};

#endif // !_VEHICLE_H_
