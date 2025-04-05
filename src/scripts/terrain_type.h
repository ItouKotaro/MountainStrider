//=============================================================
//
// 地形の種類 [terrain_type.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TERRAIN_TYPE_H_
#define _TERRAIN_TYPE_H_

// 地形ノイズ（基底）
class TerrainNoise
{
public:
	TerrainNoise();
	virtual ~TerrainNoise() {}

	// 生成する
	virtual void Generate() = 0;

	// ノイズマップビルダーを設定する
	void SetNoiseMapBuilder(utils::NoiseMapBuilderPlane* noiseMapBuilder) { m_noiseMapBuilder = noiseMapBuilder; }

	// ノイズマップを設定する
	void SetNoiseMap(utils::NoiseMap* noiseMap) { m_noiseMap = noiseMap; }
protected:
	utils::NoiseMapBuilderPlane* m_noiseMapBuilder;
	utils::NoiseMap* m_noiseMap;
	int m_terrainSize;
	float m_terrainScale;
};


// 砂漠
class DesertTerrainNoise : public TerrainNoise
{
public:
	void Generate() override;
};

#endif // !_TERRAIN_TYPE_H_
