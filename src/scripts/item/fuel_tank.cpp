//=============================================================
//
// �R���^���N [fuel_tank.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "fuel_tank.h"
#include "scripts/vehicle.h"

const float FuelTank::FUEL_VALUE = 150.0f;

//=============================================================
// [FuelTank] ������
//=============================================================
void FuelTank::Init()
{
	m_pVehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [FuelTank] �ڐG
//=============================================================
void FuelTank::OnTriggerEnter(GameObject* other)
{
	if (other == m_pVehicle)
	{ // �o�C�N�̎�
		m_pVehicle->GetComponent<CVehicle>()->AddFuel(FUEL_VALUE);
	}
}