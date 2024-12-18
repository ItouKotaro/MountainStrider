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

// パーティクルモジュール
namespace ParticleModule
{
	// 放出
	class Emission
	{
	public:
		void SetRateOverTime(const float& time) { m_rateOverTime = time; }
		float GetRateOverTime() { return m_rateOverTime; }
		void SetElapsedTime(float elapsedTime) { m_elapsedTime += elapsedTime; }
		virtual int GetResult();
	protected:
		float m_rateOverTime;	// 秒あたりの放出数
		float m_elapsedTime;		// 経過時間
	};

	// 形状
	class Shape
	{
	public:
		struct ResultData
		{
			D3DXVECTOR3 position;
			D3DXVECTOR3 direction;
		};
		virtual ResultData GetResult() = 0;
	};

	// 力
	class Power
	{
	public:
		void SetPower(const float& power) 
		{ 
			m_min = power;
			m_max = power;
		}
		void SetPower(const float& min, const float& max)
		{
			m_min = min;
			m_max = max;
		}
		virtual float GetResult() { Benlib::Random(m_min, m_max); }
	private:
		float m_min, m_max;
	};
}


// パーティクルシステム
class ParticleSystem : public Component
{
public:
	ParticleSystem() :
		m_emission(nullptr),
		m_shape(nullptr),
		m_power(nullptr){}
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	ParticleModule::Emission* m_emission;		// 放出
	ParticleModule::Shape* m_shape;				// 形状
	ParticleModule::Power* m_power;				// 力

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

// パーティクルのシェイプ
namespace ParticleShape
{
	// ポイント（一点）
	class PointShape : public ParticleModule::Shape
	{
	public:
		ResultData GetResult() override
		{
			ResultData data;
			data.position = { 0.0f, 0.0f, 0.0f };
			data.direction = { 0.0f, 0.0f, 0.0f };
			return data;
		}
	};

	// 球
	class SphereShape : public ParticleModule::Shape
	{
	public:
		ResultData GetResult() override;
		void SetRadius(const float& radius) { m_radius = radius; }
	private:
		float m_radius;	// 半径
	};
}



#endif // !_PARTICLE_H_
