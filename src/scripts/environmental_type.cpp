//=============================================================
//
// 環境タイプ [environmental_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "environmental_type.h"
#include "renderer.h"

//=============================================================
// [WeatherFog] 初期化
//=============================================================
void WeatherFog::Init()
{
	// デバイスの取得
	auto pDevice = CRenderer::GetInstance()->GetDevice();

	// フォグを設定する
	pDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	pDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_EXP);

	m_fogDensity = 0.001f;
	pDevice->SetRenderState(D3DRS_FOGDENSITY, *(DWORD*)(&m_fogDensity));
}

//=============================================================
// [WeatherFog] 終了
//=============================================================
void WeatherFog::Uninit()
{
	// デバイスの取得
	auto pDevice = CRenderer::GetInstance()->GetDevice();

	// フォグを設定する
	pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
}