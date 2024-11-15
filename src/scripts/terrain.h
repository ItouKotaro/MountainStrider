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

	// ���x�J���[��ǉ�����
	void AddHeightColor(const float& height, const D3DXCOLOR& color);

	// �X�΃J���[��ǉ�����
	//void AddSlantColor(const float& minAngle, const float& maxAngle, const D3DXCOLOR& color, const float& rate = 1.0f);

	// �w��̍��x�̐F���擾����
	D3DXCOLOR GetHeightColor(const float& height);

	// �w��̈ʒu�̐F���擾����
	D3DXCOLOR GetVertexColor(const float& x, const float& y);

	// ���x�J���[
	struct HeightColor
	{
		float height;				// ���x
		D3DXCOLOR color;		// �F
	};
	std::vector<HeightColor> m_heightColor;

	//// �p�x�J���[
	//struct SlantColor
	//{
	//	float minAngle;			// �p�x�͈́i�ŏ��j
	//	float maxAngle;			// �p�x�͈́i�ő�j
	//	D3DXCOLOR color;		// �F
	//	float rate;					// �K�p����
	//};
	//std::vector<SlantColor> m_slantColor;
};

#endif // !_TERRAIN_H_
