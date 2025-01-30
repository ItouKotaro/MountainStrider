//=============================================================
//
// �n�`�̎�� [terrain_type.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_TYPE_H_
#define _TERRAIN_TYPE_H_

// �n�`�m�C�Y�i���j
class TerrainNoise
{
public:
	TerrainNoise() : m_noiseMap(nullptr), m_noiseMapBuilder(nullptr){}
	virtual ~TerrainNoise() {}

	// ��������
	virtual void Generate() = 0;

	// �m�C�Y�}�b�v�r���_�[��ݒ肷��
	void SetNoiseMapBuilder(utils::NoiseMapBuilderPlane* noiseMapBuilder) { m_noiseMapBuilder = noiseMapBuilder; }

	// �m�C�Y�}�b�v��ݒ肷��
	void SetNoiseMap(utils::NoiseMap* noiseMap) { m_noiseMap = noiseMap; }
protected:
	utils::NoiseMapBuilderPlane* m_noiseMapBuilder;
	utils::NoiseMap* m_noiseMap;
};


// ����
class DesertTerrainNoise : public TerrainNoise
{
public:
	void Generate() override;
};

#endif // !_TERRAIN_TYPE_H_
