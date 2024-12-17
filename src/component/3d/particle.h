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
	Particle() : m_vtxBuff(nullptr), m_texture(nullptr){}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// サイズの設定
	void SetSize(const float& x, const float& y);

	// テクスチャの設定
	void SetTexture(const std::string& path);
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_texture = texture; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// 頂点バッファ
	LPDIRECT3DTEXTURE9 m_texture;					// テクスチャ
};

// パーティクルエミッター
//class ParticleEmitter : public Component
//{
//public:
//	
//private:
//	struct Particle
//	{
//
//		SingleComponent<Particle> particle;
//	};
//};

#endif // !_PARTICLE_H_
