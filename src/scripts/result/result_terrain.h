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
private:
	LPDIRECT3DVERTEXBUFFER9 m_terrainVtxBuff;	// ���_���i�n�}�j
	LPDIRECT3DTEXTURE9 m_texture;						// �e�N�X�`��

	LPDIRECT3DVERTEXBUFFER9 m_travellingVtxBuff;	// ���_���
	int m_travellingCounter;
	unsigned int m_travellingIdx;

	GameObject* m_pDebugText;
};

#endif // !_RESULT_TERRAIN_H_
