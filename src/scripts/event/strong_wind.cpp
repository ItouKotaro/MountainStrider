//=============================================================
//
// 強風 [strong_wind.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "strong_wind.h"
#include "manager.h"

//=============================================================
// [StrongWindEvent] 初期化
//=============================================================
void StrongWindEvent::Init()
{
	// バイクを取得する
	m_vehicle = GameObject::Find("Vehicle");

	// 風向きを決める
	m_angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);
	
	// 力を決める
	m_power = Benlib::RandomFloat(MIN_POWER, MAX_POWER);

	// イベント時間を設定する
	m_eventTimer = EVENT_BASE_TIME + Benlib::RandomFloat(0.0f, EVENT_RANDOM_TIME);
}

//=============================================================
// [StrongWindEvent] 終了
//=============================================================
void StrongWindEvent::Uninit()
{

}

//=============================================================
// [StrongWindEvent] 更新
//=============================================================
void StrongWindEvent::Update()
{
	// 力を加える
	CCollision::GetCollision(m_vehicle)->GetRigidBody()->applyCentralForce(
		btVector3(sinf(m_angle) * m_power, -m_power * 0.5f, cosf(m_angle) * m_power)
	);

	CCollision::GetCollision(m_vehicle)->GetRigidBody()->setAngularVelocity(
		btVector3(sinf(m_angle) * m_power * 2.0f, 0.0f, cosf(m_angle) * m_power * 2.0f)
	);

	// タイマーを進める
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();

	// イベントタイマーが終了したとき
	if (m_eventTimer <= 0.0f)
	{
		// イベントを終了する
		EndEvent();
	}
}