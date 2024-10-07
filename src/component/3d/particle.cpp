//=============================================================
//
// パーティクル [particle.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "particle.h"
#include "manager.h"

//=============================================================
// [CParticle]  コンストラクタ
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
// [CParticle]  寿命を設定する
//=============================================================
void CParticle::SetLife(const int& nLife)
{
	m_nLife = nLife;
}

//=============================================================
// [CParticle]  1フレームごとの移動量を設定する
//=============================================================
void CParticle::SetEveryMove(const D3DXVECTOR3& move)
{
	m_everyMove = move;
}

//=============================================================
// [CParticle]  1フレームごとの回転量を設定する
//=============================================================
void CParticle::SetEveryAngle(const float& fAngle)
{
	m_everyAngle = fAngle;
}

//=============================================================
// [CParticle] 1フレームごとの拡大量を設定する
//=============================================================
void CParticle::SetEverySize(const float& fSize)
{
	m_everySize = { fSize, fSize };
}

//=============================================================
// [CParticle] 1フレームごとの拡大量を設定する
//=============================================================
void CParticle::SetEverySize(const D3DXVECTOR2& size)
{
	m_everySize = size;
}

//=============================================================
// [CParticle] 1フレームごとの透明率を設定する
//=============================================================
void CParticle::SetEveryAlpha(const float& fAlpha)
{
	m_everyAlpha = fAlpha;
}

//=============================================================
// [CParticle] 更新
//=============================================================
void CParticle::Update()
{
	// 移動量を加算する
	transform->Translate(m_everyMove);

	// 回転量を加算する
	transform->Rotate(0.0f, 0.0f, m_everyAngle);

	// 拡大率を加算する
	Set(transform->GetSizeX() + m_everySize.x, transform->GetSizeY() + m_everySize.y);

	// 透明度を加算する
	SetColor({ GetColor().r, GetColor().g, GetColor().b, GetColor().a + m_everyAlpha });


	//-----------------------------------------------------------------
	// 破棄する条件

	// 1. 寿命
	m_nLife--;
	if (m_nLife <= 0)
	{ // 寿命が尽きた
		gameObject->Destroy();
		return;
	}

	// 2. 透明度
	if (GetColor().a <= 0)
	{ // 透明になったとき
		gameObject->Destroy();
		return;
	}
}