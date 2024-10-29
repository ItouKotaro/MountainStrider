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

	static const int TERRAIN_SIZE = 200;
private:
	std::array<std::array<int, TERRAIN_SIZE>, TERRAIN_SIZE> m_terrainHeight;
	unsigned short* m_heightfieldData;

	GameObject* m_pField;
	btCollisionObject* m_pCollisionObj;
};

#endif // !_TERRAIN_H_
