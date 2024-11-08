//=============================================================
//
// �n�` [terrain.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "internal/physics.h"
#include "nature_produces.h"
#include "component.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

// �n�`
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

	// �n�`�𐶐�����
	void GenerateTerrain();

	// �������𐶐�����
	void GenerateProduces();

	// ��������o�^����
	void RegisterProduces(CNatureProduces* pNatureProduce);

	std::array<std::array<int, TERRAIN_SIZE>, TERRAIN_SIZE> m_terrainHeight;	// �n�`���

	GameObject* m_pField;
	float* m_terrainData;
	btHeightfieldTerrainShape* m_terrainShape;

	// ���������X�g
	std::vector<CNatureProduces*> m_natureProduces;
};

#endif // !_TERRAIN_H_
