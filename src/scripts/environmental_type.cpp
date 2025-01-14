//=============================================================
//
// ���^�C�v [environmental_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental_type.h"
#include "renderer.h"
#include "component/3d/particle.h"
#include "render/shadow_rb.h"
#include "component/3d/collision.h"

//=============================================================
// [WeatherRain] �J�p�[�e�B�N��
//=============================================================
class RainParticle : public ParticleModule::Shape
{
public:
	ResultData GetResult() override
	{
		ResultData data;
		data.direction.x = Benlib::RandomFloat(-1.0f, 1.0f);
		data.direction.y = Benlib::RandomFloat(-10.0f, -20.0f);
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
// [WeatherRain] ������
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

	// �J������e�ɐݒ肷��
	m_rainParticle->SetParent(GameObject::Find("Camera"));

	// �n�ʂ̊���₷����ύX����
	CCollision::GetCollision(GameObject::Find("TerrainField"))->GetGhostObject()->setFriction(0.05f);

	// �J�̉����Đ�����
	m_rainSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\ENVIRONMENTAL\\rain.mp3", FMOD_2D | FMOD_LOOP_NORMAL, true);
	m_rainParticle->AddComponent<AudioSource>()->Play(m_rainSE);

	// ���̖��邳��ύX����
	D3DXVECTOR4 ambient = static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->GetAmbient() * 0.6f;
	ambient.w = 1.0f;
	static_cast<ShadowRenderBuffer*>(CRenderer::GetInstance()->GetRenderBuffer("main"))->SetAmbient(ambient);
}

//=============================================================
// [WeatherRain] �I��
//=============================================================
void WeatherRain::Uninit()
{
	m_rainParticle->Destroy();
	AudioManager::GetInstance()->RemoveClip(m_rainSE);
}

//=============================================================
// [WeatherRain] �X�V
//=============================================================
void WeatherRain::Update()
{
}

////=============================================================
//// [WeatherFog] ������
////=============================================================
//void WeatherFog::Init()
//{
//	// �f�o�C�X�̎擾
//	auto pDevice = CRenderer::GetInstance()->GetDevice();
//
//	// �t�H�O��ݒ肷��
//	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
//	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
//	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);
//
//	m_fogDensity = 0.001f;
//	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fogDensity));
//}
//
////=============================================================
//// [WeatherFog] �I��
////=============================================================
//void WeatherFog::Uninit()
//{
//	// �f�o�C�X�̎擾
//	auto pDevice = CRenderer::GetInstance()->GetDevice();
//
//	// �t�H�O��ݒ肷��
//	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
//}