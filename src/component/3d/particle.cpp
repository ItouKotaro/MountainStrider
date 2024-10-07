//=============================================================
//
// �p�[�e�B�N�� [particle.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "particle.h"
#include "manager.h"

//=============================================================
// [CParticle]  �R���X�g���N�^
//=============================================================
CParticle::CParticle(const int& nLife, const D3DXVECTOR3& move, const float& fAlpha, const D3DXVECTOR2& size, const float& fAngle)
{
	SetLife(nLife);
	SetEveryMove(move);
	SetEveryAlpha(fAlpha);
	SetEverySize(size);
	SetEveryAngle(fAngle);
}

//=============================================================
// [CParticle]  ������ݒ肷��
//=============================================================
void CParticle::SetLife(const int& nLife)
{
	m_nLife = nLife;
}

//=============================================================
// [CParticle]  1�t���[�����Ƃ̈ړ��ʂ�ݒ肷��
//=============================================================
void CParticle::SetEveryMove(const D3DXVECTOR3& move)
{
	m_everyMove = move;
}

//=============================================================
// [CParticle]  1�t���[�����Ƃ̉�]�ʂ�ݒ肷��
//=============================================================
void CParticle::SetEveryAngle(const float& fAngle)
{
	m_everyAngle = fAngle;
}

//=============================================================
// [CParticle] 1�t���[�����Ƃ̊g��ʂ�ݒ肷��
//=============================================================
void CParticle::SetEverySize(const float& fSize)
{
	m_everySize = { fSize, fSize };
}

//=============================================================
// [CParticle] 1�t���[�����Ƃ̊g��ʂ�ݒ肷��
//=============================================================
void CParticle::SetEverySize(const D3DXVECTOR2& size)
{
	m_everySize = size;
}

//=============================================================
// [CParticle] 1�t���[�����Ƃ̓�������ݒ肷��
//=============================================================
void CParticle::SetEveryAlpha(const float& fAlpha)
{
	m_everyAlpha = fAlpha;
}

//=============================================================
// [CParticle] �X�V
//=============================================================
void CParticle::Update()
{
	// �ړ��ʂ����Z����
	transform->Translate(m_everyMove);

	// ��]�ʂ����Z����
	transform->Rotate(0.0f, 0.0f, m_everyAngle);

	// �g�嗦�����Z����
	Set(transform->GetSizeX() + m_everySize.x, transform->GetSizeY() + m_everySize.y);

	// �����x�����Z����
	SetColor({ GetColor().r, GetColor().g, GetColor().b, GetColor().a + m_everyAlpha });


	//-----------------------------------------------------------------
	// �j���������

	// 1. ����
	m_nLife--;
	if (m_nLife <= 0)
	{ // �������s����
		gameObject->Destroy();
		return;
	}

	// 2. �����x
	if (GetColor().a <= 0)
	{ // �����ɂȂ����Ƃ�
		gameObject->Destroy();
		return;
	}
}