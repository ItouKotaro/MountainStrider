//=============================================================
//
// 車両のサウンド [vehicle_sound.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "vehicle_sound.h"


//=============================================================
// [VehicleSound] 初期化
//=============================================================
void VehicleSound::Init()
{
	// バイクコンポーネントを取得する
	m_vehicle = gameObject->GetComponent<CVehicle>();

	// 走行音
	m_runningSE = (new GameObject())->AddComponent<CSound>();
	m_runningSE->transform->Translate(300.0f, 50.0f, 0.0f);
	m_runningSE->gameObject->SetParent(gameObject);
	m_runningSE->LoadWav("data\\SOUND\\SE\\BIKE\\running00.wav");
	m_runningSE->SetVolume(100.0f);
	m_runningSE->Play();
	m_runningSE->SetLoop(true);
}

//=============================================================
// [VehicleSound] 終了
//=============================================================
void VehicleSound::Uninit()
{

}

//=============================================================
// [VehicleSound] 更新
//=============================================================
void VehicleSound::Update()
{
}