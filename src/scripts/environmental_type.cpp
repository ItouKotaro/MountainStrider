//=============================================================
//
// ���^�C�v [environmental_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental_type.h"
#include "renderer.h"

//=============================================================
// [WeatherFog] ������
//=============================================================
void WeatherFog::Init()
{
	// �f�o�C�X�̎擾
	auto pDevice = CRenderer::GetInstance()->GetDevice();

	// �t�H�O��ݒ肷��
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);

	m_fogDensity = 0.001f;
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fogDensity));
}

//=============================================================
// [WeatherFog] �I��
//=============================================================
void WeatherFog::Uninit()
{
	// �f�o�C�X�̎擾
	auto pDevice = CRenderer::GetInstance()->GetDevice();

	// �t�H�O��ݒ肷��
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}