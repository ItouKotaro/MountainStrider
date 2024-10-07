//=============================================================
//
// パーティクル [particle.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _PARTICLE_H_
#define _PARTICLE_H_

#include "billboard.h"

// パーティクルコンポーネント
class CParticle : public CBillboard
{
public:
	CParticle(const int& nLife = 10, const D3DXVECTOR3& move = { 0.0f, 0.0f, 0.0f }, const float& fAlpha = 0.0f, const D3DXVECTOR2& size = {0.0f, 0.0f}, const float& fAngle = 0.0f);
	//CParticle(const int& nLife = 10, const D3DXVECTOR3& move = { 0.0f, 0.0f, 0.0f }, const float& fAlpha = 0.0f, const float& fSize = 0.0f, const float& fAngle = 0.0f);

	// 寿命を設定する
	void SetLife(const int& nLife);

	// 1フレームごとの移動量を設定する
	void SetEveryMove(const D3DXVECTOR3& move);

	// 1フレームごとの回転量を設定する
	void SetEveryAngle(const float& fAngle);

	// 1フレームごとの拡大量を設定する
	void SetEverySize(const float& fSize);
	void SetEverySize(const D3DXVECTOR2& size);

	// 1フレームごとの透明率を設定する
	void SetEveryAlpha(const float& fAlpha);


	void Update() override;				// 更新
private:
	int m_nLife;								// 寿命
	D3DXVECTOR3 m_everyMove;	// 移動量
	D3DXVECTOR2 m_everySize;		// 拡大
	float m_everyAngle, m_everyAlpha;
};

#endif // !_PARTICLE_H_
