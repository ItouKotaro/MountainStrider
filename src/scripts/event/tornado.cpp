//=============================================================
//
// 竜巻イベント [tornado.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "tornado.h"
#include "manager.h"
#include "component/3d/particle.h"

//=============================================================
// トルネード用パーティクル
//=============================================================
class TornadoParticle : public ParticleModule::Shape
{
public:
	ResultData GetResult()
	{
		ResultData data;
		
		// 位置
		float height = Benlib::RandomFloat(0.0f, 500.0f);
		float radius = (height * height) / 10.0f;
		float angle = Benlib::RandomFloat(0.0f, D3DX_PI * 2.0f);

		data.position.x = sinf(angle) * radius;
		data.position.y = height;
		data.position.z = cosf(angle) * radius;
	}
};

//=============================================================
// [TornadoEvent] 初期化
//=============================================================
void TornadoEvent::Init()
{
	// パーティクルを作成する
	m_tornade = new GameObject();
	m_tornade->AddComponent<ParticleSystem>();
}

//=============================================================
// [TornadoEvent] 終了
//=============================================================
void TornadoEvent::Uninit()
{

}

//=============================================================
// [TornadoEvent] 更新
//=============================================================
void TornadoEvent::Update()
{
	// タイマーを進める
	m_eventTimer -= CManager::GetInstance()->GetDeltaTime();

	// イベントタイマーが終了したとき
	if (m_eventTimer <= 0.0f)
	{
		// イベントを終了する
		EndEvent();
	}
}