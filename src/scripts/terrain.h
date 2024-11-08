//=============================================================
//
// 地形 [terrain.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "internal/physics.h"
#include "nature_produces.h"
#include "component.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

// 地形
class CTerrain : public Component
{
public:
	void Init();
	void Uninit();
	void Generate();

	static const int TERRAIN_SIZE = 10;
	static const float TERRAIN_SCALE;
private:
	void UninitTerrain();

	// 地形を生成する
	void GenerateTerrain();

	// 生成物を生成する
	void GenerateProduces();

	// 生成物を登録する
	void RegisterProduces(CNatureProduces* pNatureProduce);

	std::array<std::array<int, TERRAIN_SIZE>, TERRAIN_SIZE> m_terrainHeight;	// 地形情報

	GameObject* m_pField;
	float* m_terrainData;
	btHeightfieldTerrainShape* m_terrainShape;

	// 生成物リスト
	std::vector<CNatureProduces*> m_natureProduces;
};

#endif // !_TERRAIN_H_
