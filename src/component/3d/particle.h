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
	struct Range
	{
		float min;
		float max;
	};
	struct RangeInt
	{
		int min;
		int max;
	};

	// パーティクルデータ
	struct ParticleData
	{
		SingleComponent<Particle>* particle;		// パーティクル
		D3DXVECTOR3 move;							// 動き
		D3DXCOLOR color;									// 色
		float size;												// サイズ

		int lifeCounter;										// 寿命
		int destroyCounter;								// 破棄カウンター
		bool use;												// 使用しているか
	};

	ParticleSystem() :
		m_gravity(0.0f),
		m_size({ 5.0f, 5.0f }),
		m_angle({ 0.0f, 0.0f }),
		m_fluctuationSize(0.0f),
		m_fluctuationAlpha(0.0f),
		m_color(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f)),
		m_emission(nullptr),
		m_shape(nullptr),
		m_power({1.0f, 1.0f}),
		m_lifetime({60, 60}),
		m_texture(nullptr),
		m_vortex(0.0f){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// @brief 重力の設定
	void SetGravity(const float& gravity) { m_gravity = -gravity; }
	// @brief 重力の取得
	float GetGravity() { return m_gravity; }

	//@brief 回転の設定
	void SetAngle(const float& angle) { m_angle = { angle, angle }; }
	void SetAngle(const float& min, const float& max) { m_angle = { min, max }; }
	//@brief 回転の取得
	Range GetAngle() { return m_angle; }

	//@brief カラーの設定
	void SetColor(const D3DXCOLOR& color) { m_color = color; }
	//@brief カラーの取得
	D3DXCOLOR GetColor() { return m_color; }

	//@brief サイズの設定
	void SetSize(const float& size) { m_size = { size , size }; }
	void SetSize(const float& min, const float& max) { m_size = { min, max }; }
	//@brief サイズの取得
	Range GetSize() { return m_size; }

	//@brief 力の設定
	void SetPower(const float& power) { m_power = { power, power }; }
	void SetPower(const float& min, const float& max) { m_power = { min, max }; }
	//@brief 力の取得
	Range GetPower() { return m_power; }

	//@brief 生存時間の設定
	void SetLifetime(const int& time) { m_lifetime = { time, time }; }
	void SetLifetime(const int& min, const int& max) { m_lifetime = { min, max }; }
	//@brief 生存時間の取得
	RangeInt GetLifetime() { return m_lifetime; }

	//@brief サイズの変動設定
	void SetFluctuationSize(const float& value) { m_fluctuationSize = value; }
	//@brief サイズの変動取得
	float GetFluctuationSize() { return m_fluctuationSize; }

	//@brief 透明度の変動設定
	void SetFluctuationAlpha(const float& value) { m_fluctuationAlpha = value; }
	//@brief 透明度の変動取得
	float GetFluctuationAlpha() { return m_fluctuationAlpha; }

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

	// @brief 渦の設定
	void SetVortex(const float& value) { m_vortex = value; }
	// @brief 渦の取得
	float GetVortex() { return m_vortex; }

	// @brief 移動式の設定
	void SetMoveEquation(std::function<D3DXVECTOR3(ParticleSystem::ParticleData*)> equation) { m_moveEquation = equation; }

	// @brief パーティクルの総数
	int GetNumParticles() { return static_cast<int>(m_particleData.size()); }

	static const int DESTROY_TIME = 60;
	const float RENDER_DISTANCE = 800.0f;
private:
	void UpdateGenerator();
	void UpdateParticles();
	void UpdateDestroy();

	// モジュール
	ParticleModule::Emission* m_emission;		// 放出
	ParticleModule::Shape* m_shape;				// 形状
	ParticleModule::Texture* m_texture;			// テクスチャ

	// 標準設定
	float m_gravity;											// 重力
	Range m_size;											// サイズ
	float m_fluctuationSize;								// サイズの変動
	D3DXCOLOR m_color;								// カラー
	float m_fluctuationAlpha;							// 透明度の変動
	Range m_angle;											// 回転
	Range m_power;										// 力
	RangeInt m_lifetime;									// 生存時間
	float m_vortex;											// 渦

	std::function<D3DXVECTOR3(ParticleSystem::ParticleData*)> m_moveEquation;

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
