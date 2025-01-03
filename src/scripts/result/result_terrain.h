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
private:
	LPDIRECT3DVERTEXBUFFER9 m_terrainVtxBuff;	// 頂点情報（地図）
	LPDIRECT3DTEXTURE9 m_texture;						// テクスチャ

	LPDIRECT3DVERTEXBUFFER9 m_travellingVtxBuff;	// 頂点情報
	int m_travellingCounter;
	unsigned int m_travellingIdx;
};

#endif // !_RESULT_TERRAIN_H_
