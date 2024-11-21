//=============================================================
//
// 燃料タンク [fuel_tank.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "fuel_tank.h"
#include "scripts/vehicle.h"

const float FuelTank::FUEL_VALUE = 150.0f;

//=============================================================
// [FuelTank] 初期化
//=============================================================
void FuelTank::Init()
{
	m_pVehicle = GameObject::Find("Vehicle");
}

//=============================================================
// [FuelTank] 接触
//=============================================================
void FuelTank::OnTriggerEnter(GameObject* other)
{
	if (other == m_pVehicle)
	{ // バイクの時
		m_pVehicle->GetComponent<CVehicle>()->AddFuel(FUEL_VALUE);
	}
}