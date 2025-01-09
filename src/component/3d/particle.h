//=============================================================
//
// �p�[�e�B�N�� [particle.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "component.h"
#include "component/3d/billboard.h"

// �p�[�e�B�N���i�P�́j
class Particle : public Component
{
public:
	Particle() : m_vtxBuff(nullptr), m_texture(nullptr), m_color({1.0f, 1.0f, 1.0f, 1.0f}) {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// �T�C�Y�̐ݒ�
	void SetSize(const float& x, const float& y);

	// �J���[�̐ݒ�
	void SetColor(const D3DXCOLOR& color) { m_color = color; }

	// �e�N�X�`���̐ݒ�
	void SetTexture(const std::string& path);
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_texture = texture; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_texture;					// �e�N�X�`��
	D3DXCOLOR m_color;
};

// �p�[�e�B�N�����W���[��
namespace ParticleModule
{
	// ���o
	class Emission
	{
	public:
		Emission() : m_rateOverTime(1.0f), m_elapsedTime(0.0f) {}
		void SetRateOverTime(const float& perSec) { m_rateOverTime = 1.0f / perSec; }
		float GetRateOverTime() { return m_rateOverTime; }
		void SetElapsedTime(float elapsedTime) { m_elapsedTime += elapsedTime; }
		virtual int GetResult();
	protected:
		float m_rateOverTime;	// �b������̕��o��
		float m_elapsedTime;		// �o�ߎ���
	};

	// �`��
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

	// �e�N�X�`��
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


// �p�[�e�B�N���V�X�e��
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
		m_texture(nullptr){}
	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// @brief �d�͂̐ݒ�
	void SetGravity(const float& gravity) { m_gravity = -gravity; }
	// @brief �d�͂̎擾
	float GetGravity() { return m_gravity; }

	//@brief ��]�̐ݒ�
	void SetAngle(const float& angle) { m_angle = { angle, angle }; }
	void SetAngle(const float& min, const float& max) { m_angle = { min, max }; }
	//@brief ��]�̎擾
	Range GetAngle() { return m_angle; }

	//@brief �J���[�̐ݒ�
	void SetColor(const D3DXCOLOR& color) { m_color = color; }
	//@brief �J���[�̎擾
	D3DXCOLOR GetColor() { return m_color; }

	//@brief �T�C�Y�̐ݒ�
	void SetSize(const float& size) { m_size = { size , size }; }
	void SetSize(const float& min, const float& max) { m_size = { min, max }; }
	//@brief �T�C�Y�̎擾
	Range GetSize() { return m_size; }

	//@brief �͂̐ݒ�
	void SetPower(const float& power) { m_power = { power, power }; }
	void SetPower(const float& min, const float& max) { m_power = { min, max }; }
	//@brief �͂̎擾
	Range GetPower() { return m_power; }

	//@brief �������Ԃ̐ݒ�
	void SetLifetime(const int& time) { m_lifetime = { time, time }; }
	void SetLifetime(const int& min, const int& max) { m_lifetime = { min, max }; }
	//@brief �������Ԃ̎擾
	RangeInt GetLifetime() { return m_lifetime; }

	//@brief �T�C�Y�̕ϓ��ݒ�
	void SetFluctuationSize(const float& value) { m_fluctuationSize = value; }
	//@brief �T�C�Y�̕ϓ��擾
	float GetFluctuationSize() { return m_fluctuationSize; }

	//@brief �����x�̕ϓ��ݒ�
	void SetFluctuationAlpha(const float& value) { m_fluctuationAlpha = value; }
	//@brief �����x�̕ϓ��擾
	float GetFluctuationAlpha() { return m_fluctuationAlpha; }

	// @brief �G�~�b�V�����̐ݒ�
	void SetEmission(ParticleModule::Emission* emission);
	// @brief �G�~�b�V�����̎擾
	ParticleModule::Emission* GetEmission() { return m_emission; }

	// @brief �V�F�C�v�̐ݒ�
	void SetShape(ParticleModule::Shape* shape);
	// @brief �V�F�C�v�̎擾
	ParticleModule::Shape* GetShape() { return m_shape; }

	// @brief �e�N�X�`���̎擾
	ParticleModule::Texture* GetTexture() { return m_texture; }

	// @brief �p�[�e�B�N���̑���
	int GetNumParticles() { return static_cast<int>(m_particleData.size()); }

	static const int DESTROY_TIME = 60;
private:
	void UpdateGenerator();
	void UpdateParticles();
	void UpdateDestroy();

	// ���W���[��
	ParticleModule::Emission* m_emission;		// ���o
	ParticleModule::Shape* m_shape;				// �`��
	ParticleModule::Texture* m_texture;			// �e�N�X�`��

	// �W���ݒ�
	float m_gravity;											// �d��
	Range m_size;											// �T�C�Y
	float m_fluctuationSize;								// �T�C�Y�̕ϓ�
	D3DXCOLOR m_color;								// �J���[
	float m_fluctuationAlpha;							// �����x�̕ϓ�
	Range m_angle;											// ��]
	Range m_power;										// ��
	RangeInt m_lifetime;									// ��������

	// �p�[�e�B�N���f�[�^
	struct ParticleData
	{
		SingleComponent<Particle>* particle;		// �p�[�e�B�N��
		D3DXVECTOR3 move;							// ����
		D3DXCOLOR color;									// �F
		float size;												// �T�C�Y

		int lifeCounter;										// ����
		int destroyCounter;								// �j���J�E���^�[
		bool use;												// �g�p���Ă��邩
	};
	std::vector<ParticleData> m_particleData;
};

// �p�[�e�B�N���̃V�F�C�v
namespace ParticleShape
{
	// �|�C���g�i��_�j
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

	// ��
	class SphereShape : public ParticleModule::Shape
	{
	public:
		SphereShape() : m_radius(1.0f) {}
		ResultData GetResult() override;
		void SetRadius(const float& radius) { m_radius = radius; }
	private:
		float m_radius;	// ���a
	};
}



#endif // !_PARTICLE_H_
