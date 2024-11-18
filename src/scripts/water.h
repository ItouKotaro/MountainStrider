//=============================================================
//
// 水 [water.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _WATER_H_
#define _WATER_H_

#include "component.h"
#include "component/2d/polygon.h"

class CWater : public CPolygon
{
public:
	CWater();
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override {}
	void Draw() override;
	LPDIRECT3DTEXTURE9 DrawTexture();

	void SetSpringPower(float SpringPower);
	void AddWave(float x, float y, float height);
private:
	GameObject* m_pField;
	LPD3DXEFFECT m_pEffect;
	D3DXHANDLE m_pTechnique, m_pTexOffset, m_pSpringPower, m_pAddWavePos, m_pAddWaveHeight;

	//波マップ
	LPDIRECT3DTEXTURE9      m_pWaveMapTexture[2];
	LPDIRECT3DSURFACE9      m_pWaveMapSurface[2];

	//法線マップ
	LPDIRECT3DTEXTURE9      m_pBumpMapTexture;
	LPDIRECT3DSURFACE9      m_pBumpMapSurface;

	//波マップのインデックス
	int m_RenderTargetIndex;
};

#endif // !_WATER_H_
