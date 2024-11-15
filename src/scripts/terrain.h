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

	static const int TERRAIN_SIZE = 100;
	static const float TERRAIN_SCALE;
private:
	void UninitTerrain();

	// 地形を生成する
	void GenerateTerrain();

	// 山道を生成する
	void GenerateRoad();

	// 生成物を生成する
	void GenerateProduces();

	// 生成物を登録する
	void RegisterProduces(CNatureProduces* pNatureProduce);

	GameObject* m_pField;
	float* m_terrainData;
	btHeightfieldTerrainShape* m_terrainShape;

	// 生成物リスト
	std::vector<CNatureProduces*> m_natureProduces;

	// 山の色（高度）
	void AddHeightColor(const float& height, const D3DXCOLOR& color);

	// 指定の高度の色を取得する
	D3DXCOLOR GetHeightColor(const float& height);

	// 高度カラー
	struct HeightColor
	{
		float height;
		D3DXCOLOR color;
	};
	std::vector<HeightColor> m_heightColor;
};

#endif // !_TERRAIN_H_
