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

namespace ParticleModule
{
	// ���o
	class Emission
	{
	};
}


// �p�[�e�B�N���G�~�b�^�[
class ParticleEmitter : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	ParticleModule::Emission m_emission;			// ���o


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



class SphereEmission : public ParticleModule::Emission
{
public:

private:

};



#endif // !_PARTICLE_H_
