//=============================================================
//
// �n�` [terrain.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "internal/physics.h"
#include "component.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"
#include "terrain_type.h"

// �n�`
class Terrain
{
public:
	void Init();
	void Uninit();
	void Update();
	void Generate();
	void SetSeed(const unsigned int& seed) { m_seed = seed; }
	unsigned int GetSeed() { return m_seed; }
	float GetMinHeight() { return m_minHeight; }
	float GetMaxHeight() { return m_maxHeight; }
	void LoadTerrainFile(const std::string path);

	// �w��̍��x�̐F���擾����
	D3DXCOLOR GetHeightColor(const float& height);
	// �w��̈ʒu�̐F���擾����
	D3DXCOLOR GetVertexColor(const int& x, const int& y);
	// ���_�̍������擾����
	float GetVertexHeight(const int& x, const int& y);

	static const int TERRAIN_SIZE = 150;
	static const float TERRAIN_SCALE;
	static const float TERRAIN_DISTANCE;
	static const float TERRAIN_DISTANCE_HALF;
private:
	void UninitTerrain();
	void GenerateGem();

	// ��
	enum Area
	{
		NONE,
		TOP,
		BOTTOM,
		LEFT,
		RIGHT
	};

	Area m_startArea;

	unsigned int m_seed;										// �V�[�h�l
	TerrainNoise* m_terrainNoise;							// �n�`�m�C�Y�f�[�^
	GameObject* m_field;									// ���b�V���t�B�[���h
	GameObject* m_limitField;								// �����t�B�[���h
	float* m_terrainData;										// ���x�f�[�^
	btHeightfieldTerrainShape* m_terrainShape;	// �n�`�R���W�����V�F�C�v
	int m_gemNum;												// �W�F���̐�

	float m_maxHeight;	// �ō����x
	float m_minHeight;	// �Œፂ�x

	bool m_lakeEnabled;	// �΂̗L�����
	float m_lakeHeight;	// �΂̍���

	int m_terrainID;			// �n�`ID

	// ���_�̍�����ݒ肷��
	void SetVertexHeight(const int& x, const int& y, const float& height);

	// �n�`�𐶐�����
	void GenerateTerrain();
	// �R���𐶐�����
	void GenerateRoad();

	std::vector<int[TERRAIN_SIZE][TERRAIN_SIZE]> m_routeData;

	// ���x�J���[��ǉ�����
	void AddHeightColor(const float& height, const D3DXCOLOR& color);
	// �X�΃J���[��ǉ�����
	void AddSlantColor(const float& minHeight, const float& maxHeight, const D3DXCOLOR& color, const float& rate = 1.0f);

	// ���x�J���[
	struct HeightColor
	{
		float height;				// ���x
		D3DXCOLOR color;		// �F
	};
	std::vector<HeightColor> m_heightColor;

	// �p�x�J���[
	struct SlantColor
	{
		float minHeight;			// ���x�͈́i�ŏ��j
		float maxHeight;		// ���x�͈́i�ő�j
		D3DXCOLOR color;		// �F
		float rate;					// �K�p����
	};
	std::vector<SlantColor> m_slantColor;
};

#endif // !_TERRAIN_H_
