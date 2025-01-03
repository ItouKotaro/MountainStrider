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
		Emission() : m_rateOverTime(1.0f), m_elapsedTime(0.0f) {}
		void SetRateOverTime(const float& perSec) { m_rateOverTime = 1.0f / perSec; }
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
		Power() : m_min(1.0f), m_max(1.0f){}
		void Set(const float& power) 
		{ 
			m_min = power;
			m_max = power;
		}
		void Set(const float& min, const float& max)
		{
			m_min = min;
			m_max = max;
		}
		virtual float GetResult() { return Benlib::RandomFloat(m_min, m_max); }
	private:
		float m_min, m_max;
	};

	// 生存時間
	class Lifetime
	{
	public:
		Lifetime() : m_min(60), m_max(60) {}
		void Set(const int& time)
		{
			m_min = time;
			m_max = time;
		}
		void Set(const int& min, const int& max)
		{
			m_min = min;
			m_max = max;
		}
		virtual int GetResult() { return Benlib::RandomInt(m_min, m_max); }
	private:
		int m_min, m_max;
	};

	// テクスチャ
	class Texture
	{
	public:
		void AddTexture(const std::string& path, const int& chance = 1);
		LPDIRECT3DTEXTURE9 GetResult();
	private:
		struct TextureData
		{
			LPDIRECT3DTEXTURE9 texture;
			int chance;
		};
		std::vector<TextureData> m_textures;
	};
}


// パーティクルシステム
class ParticleSystem : public Component
{
public:
	ParticleSystem() :
		m_gravity(0.0f),
		m_size(5.0f),
		m_emission(nullptr),
		m_shape(nullptr),
		m_power(nullptr),
		m_lifetime(nullptr),
		m_texture(nullptr){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// @brief 重力の設定
	void SetGravity(const float& gravity) { m_gravity = -gravity; }
	// @brief 重力の取得
	float GetGravity() { return m_gravity; }

	// @brief エミッションの設定
	void SetEmission(ParticleModule::Emission* emission);
	// @brief エミッションの取得
	ParticleModule::Emission* GetEmission() { return m_emission; }

	// @brief シェイプの設定
	void SetShape(ParticleModule::Shape* shape);
	// @brief シェイプの取得
	ParticleModule::Shape* GetShape() { return m_shape; }

	// @brief テクスチャの取得
	ParticleModule::Texture* GetTexture() { return m_texture; }

	// @brief パーティクルの総数
	int GetNumParticles() { return static_cast<int>(m_particleData.size()); }

	static const int DESTROY_TIME = 60;
private:
	void UpdateGenerator();
	void UpdateParticles();
	void UpdateDestroy();

	// モジュール
	ParticleModule::Emission* m_emission;		// 放出
	ParticleModule::Shape* m_shape;				// 形状
	ParticleModule::Power* m_power;				// 力
	ParticleModule::Lifetime* m_lifetime;			// 生存時間
	ParticleModule::Texture* m_texture;			// テクスチャ

	// 標準設定
	float m_gravity;											// 重力
	float m_size;												// サイズ

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
		SphereShape() : m_radius(1.0f) {}
		ResultData GetResult() override;
		void SetRadius(const float& radius) { m_radius = radius; }
	private:
		float m_radius;	// 半径
	};
}



#endif // !_PARTICLE_H_
