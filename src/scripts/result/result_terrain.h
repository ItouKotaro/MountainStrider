//=============================================================
//
// ���U���g�̒n�`�\�� [result_terrain.h]
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

	// �n�}�f�[�^
	LPDIRECT3DVERTEXBUFFER9 m_terrainVtxBuff;	// ���_���i�n�}�j
	LPDIRECT3DTEXTURE9 m_texture;						// �e�N�X�`��

	// ���s�f�[�^
	LPDIRECT3DVERTEXBUFFER9 m_travellingVtxBuff;	// ���_���
	int m_travellingCounter;
	unsigned int m_travellingIdx;

	//	�r�f�[�^
	LPDIRECT3DVERTEXBUFFER9 m_lakeVtxBuff;	// ���_���
	int m_numLake;

	const float MAP_ROLL = 0.0f;
	const float LAKE_ROLL = D3DX_PI;
};

#endif // !_RESULT_TERRAIN_H_
