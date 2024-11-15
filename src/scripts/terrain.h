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

	static const int TERRAIN_SIZE = 100;
	static const float TERRAIN_SCALE;
private:
	void UninitTerrain();

	// �n�`�𐶐�����
	void GenerateTerrain();

	// �R���𐶐�����
	void GenerateRoad();

	// �������𐶐�����
	void GenerateProduces();

	// ��������o�^����
	void RegisterProduces(CNatureProduces* pNatureProduce);

	GameObject* m_pField;
	float* m_terrainData;
	btHeightfieldTerrainShape* m_terrainShape;

	// ���������X�g
	std::vector<CNatureProduces*> m_natureProduces;

	// �R�̐F�i���x�j
	void AddHeightColor(const float& height, const D3DXCOLOR& color);

	// �w��̍��x�̐F���擾����
	D3DXCOLOR GetHeightColor(const float& height);

	// ���x�J���[
	struct HeightColor
	{
		float height;
		D3DXCOLOR color;
	};
	std::vector<HeightColor> m_heightColor;
};

#endif // !_TERRAIN_H_
