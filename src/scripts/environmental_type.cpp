//=============================================================
//
// 環境タイプ [environmental_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental_type.h"
#include "renderer.h"
#include "component/3d/particle.h"
#include "render/shadow_rb.h"
#include "component/3d/collision.h"

//=============================================================
// [WeatherRain] 雨パーティクル
//=============================================================
class RainParticle : public ParticleModule::Shape
{
public:
	ResultData GetResult() override
	{
		ResultData data;
		data.direction.x = Benlib::RandomFloat(-1.0f, 1.0f);
		data.direction.y = Benlib::RandomFloat(-20.0f, -10.0f);
		data.direction.z = Benlib::RandomFloat(-1.0f, 1.0f);

		data.position.x = Benlib::RandomFloat(-RADIUS, RADIUS);
		data.position.y = 400.0f;
		data.position.z = Benlib::RandomFloat(-RADIUS, RADIUS);
		return data;
	}
private:
	const float RADIUS = 300.0f;
};

//=============================================================
// [WeatherRain] 初期化
//=============================================================
void WeatherRain::Init()
{
	m_rainParticle = new GameObject();
	auto particle = m_rainParticle->AddComponent<ParticleSystem>();
	particle->SetGravity(0.09f);
	particle->SetShape(new RainParticle());
	particle->GetEmission()->SetRateOverTime(200.0f);
	particle->SetLifetime(180, 180);
	particle->GetTexture()->AddTexture("data\\TEXTURE\\PARTICLE\\rain.png");
	particle->SetSize(5.0f, 9.0f);

	// カメラを親に設定する
	m_rainParticle->SetParent(GameObject::Find("Camera"));

	// 地面の滑りやすさを変更する
	CCollision::GetCollision(GameObject::Find("TerrainField"))->GetGhostObject()->setFriction(0.05f);

	// 雨の音を再生する
	m_rainSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\ENVIRONMENTAL\\rain.mp3", FMOD_2D | FMOD_LOOP_NORMAL, true);
	m_rainParticle->AddComponent<AudioSource>()->Play(m_rainSE);
}

//=============================================================
// [WeatherRain] 終了
//=============================================================
void WeatherRain::Uninit()
{
	m_rainParticle->Destroy();
	AudioManager::GetInstance()->RemoveClip(m_rainSE);
}

//=============================================================
// [WeatherRain] 更新
//=============================================================
void WeatherRain::Update()
{
}

////=============================================================
//// [WeatherFog] 初期化
////=============================================================
//void WeatherFog::Init()
//{
//	// デバイスの取得
//	auto pDevice = CRenderer::GetInstance()->GetDevice();
//
//	// フォグを設定する
//	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
//
//	m_fogDensity = 0.001f;
//	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fogDensity));
//}
//
////=============================================================
//// [WeatherFog] 終了
////=============================================================
//void WeatherFog::Uninit()
//{
//	// デバイスの取得
//	auto pDevice = CRenderer::GetInstance()->GetDevice();
//
//	// フォグを設定する
//	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
//}