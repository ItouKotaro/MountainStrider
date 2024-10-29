//=============================================================
//
// ínå` [terrain.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "internal/physics.h"
#include "component.h"

class CTerrain : public Component
{
public:
	void Init();
	void Uninit();
	void Generate();

	static const int TERRAIN_SIZE = 150;
private:
	std::array<std::array<int, TERRAIN_SIZE>, TERRAIN_SIZE> m_terrainHeight;
	int* m_indices;
	float* m_vertices;

	GameObject* m_pField;
	btStridingMeshInterface* m_pMeshData;
	btConvexTriangleMeshShape* m_pMeshCollision;
};

#endif // !_TERRAIN_H_
