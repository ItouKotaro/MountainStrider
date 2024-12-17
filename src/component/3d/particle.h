//=============================================================
//
// パーティクル [particle.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "component.h"
#include "component/3d/billboard.h"

// パーティクル（単体）
class Particle : public Component
{
public:
	Particle() : m_vtxBuff(nullptr), m_texture(nullptr), m_color({1.0f, 1.0f, 1.0f, 1.0f}) {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// サイズの設定
	void SetSize(const float& x, const float& y);

	// カラーの設定
	void SetColor(const D3DXCOLOR& color) { m_color = color; }

	// テクスチャの設定
	void SetTexture(const std::string& path);
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_texture = texture; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_texture;					// テクスチャ
	D3DXCOLOR m_color;
};

namespace ParticleModule
{
	// 放出
	class Emission
	{
	};
}


// パーティクルエミッター
class ParticleEmitter : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	ParticleModule::Emission m_emission;			// 放出


	// パーティクルデータ
	struct ParticleData
	{
		SingleComponent<Particle>* particle;		// パーティクル
		D3DXVECTOR3 move;							// 動き
		int lifeCounter;										// 寿命
		int destroyCounter;								// 破棄カウンター
		bool use;												// 使用しているか
	};
	std::vector<ParticleData> m_particleData;
};



class SphereEmission : public ParticleModule::Emission
{
public:

private:

};



#endif // !_PARTICLE_H_
