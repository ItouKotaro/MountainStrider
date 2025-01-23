//=============================================================
//
// リザルトの地形表示 [result_terrain.h]
// Author: Ito Kotaro
// 
//=============================================================
#ifndef _RESULT_TERRAIN_H_
#define _RESULT_TERRAIN_H_

#include "component.h"

class ResultTerrain : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DrawUI() override;

	static const std::string TERRAIN_TEX;
	static const int SIZE;
	static const float TRAVELLING_POINT_SIZE;
	static const int TRAVELLING_FRAME;
	static const int TRAVELLING_ENDFRAME;
	static const float POINT_SIZE;
private:
	void InitLakeMap();

	// 地図データ
	LPDIRECT3DVERTEXBUFFER9 m_terrainVtxBuff;	// 頂点情報（地図）
	LPDIRECT3DTEXTURE9 m_texture;						// テクスチャ

	// 走行データ
	LPDIRECT3DVERTEXBUFFER9 m_travellingVtxBuff;	// 頂点情報
	int m_travellingCounter;
	unsigned int m_travellingIdx;

	//	池データ
	LPDIRECT3DVERTEXBUFFER9 m_lakeVtxBuff;	// 頂点情報
	int m_numLake;

	const float MAP_ROLL = 0.0f;
	const float LAKE_ROLL = D3DX_PI;
};

#endif // !_RESULT_TERRAIN_H_
