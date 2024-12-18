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
		void SetRateOverTime(const float& time) { m_rateOverTime = time; }
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

	// ��
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


// �p�[�e�B�N���V�X�e��
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
	ParticleModule::Emission* m_emission;		// ���o
	ParticleModule::Shape* m_shape;				// �`��
	ParticleModule::Power* m_power;				// ��

	// �p�[�e�B�N���f�[�^
	struct ParticleData
	{
		SingleComponent<Particle>* particle;		// �p�[�e�B�N��
		D3DXVECTOR3 move;							// ����
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
		ResultData GetResult() override;
		void SetRadius(const float& radius) { m_radius = radius; }
	private:
		float m_radius;	// ���a
	};
}



#endif // !_PARTICLE_H_
