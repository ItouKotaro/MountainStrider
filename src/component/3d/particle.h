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
	Particle() : m_vtxBuff(nullptr), m_texture(nullptr){}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void Draw() override;

	// �T�C�Y�̐ݒ�
	void SetSize(const float& x, const float& y);

	// �e�N�X�`���̐ݒ�
	void SetTexture(const std::string& path);
	void BindTexture(LPDIRECT3DTEXTURE9 texture) { m_texture = texture; }
private:
	LPDIRECT3DVERTEXBUFFER9 m_vtxBuff;		// ���_�o�b�t�@
	LPDIRECT3DTEXTURE9 m_texture;					// �e�N�X�`��
};

// �p�[�e�B�N���G�~�b�^�[
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
