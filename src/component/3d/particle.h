//=============================================================
//
// �p�[�e�B�N�� [particle.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "billboard.h"

// �p�[�e�B�N���R���|�[�l���g
class CParticle : public CBillboard
{
public:
	CParticle(const int& nLife = 10, const D3DXVECTOR3& move = { 0.0f, 0.0f, 0.0f }, const float& fAlpha = 0.0f, const D3DXVECTOR2& size = {0.0f, 0.0f}, const float& fAngle = 0.0f);
	//CParticle(const int& nLife = 10, const D3DXVECTOR3& move = { 0.0f, 0.0f, 0.0f }, const float& fAlpha = 0.0f, const float& fSize = 0.0f, const float& fAngle = 0.0f);

	// ������ݒ肷��
	void SetLife(const int& nLife);

	// 1�t���[�����Ƃ̈ړ��ʂ�ݒ肷��
	void SetEveryMove(const D3DXVECTOR3& move);

	// 1�t���[�����Ƃ̉�]�ʂ�ݒ肷��
	void SetEveryAngle(const float& fAngle);

	// 1�t���[�����Ƃ̊g��ʂ�ݒ肷��
	void SetEverySize(const float& fSize);
	void SetEverySize(const D3DXVECTOR2& size);

	// 1�t���[�����Ƃ̓�������ݒ肷��
	void SetEveryAlpha(const float& fAlpha);


	void Update() override;				// �X�V
private:
	int m_nLife;								// ����
	D3DXVECTOR3 m_everyMove;	// �ړ���
	D3DXVECTOR2 m_everySize;		// �g��
	float m_everyAngle, m_everyAlpha;
};

#endif // !_PARTICLE_H_
