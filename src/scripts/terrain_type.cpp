//=============================================================
//
// 地形の種類 [terrain_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain_type.h"
#include "terrain.h"

//=============================================================
// [DesertTerrainNoise] 生成する
//=============================================================
void DesertTerrainNoise::Generate()
{
	// パーリンノイズを生成する
	module::Perlin myModule;
	myModule.SetSeed(rand());

	// スケールを変更する
	module::ScaleBias scaled;
	scaled.SetSourceModule(0, myModule);
	scaled.SetScale(100.0f);

	// 生成する
	m_noiseMapBuilder->SetSourceModule(scaled);
	m_noiseMapBuilder->SetDestNoiseMap(*m_noiseMap);
	m_noiseMapBuilder->SetDestSize(Terrain::TERRAIN_SIZE, Terrain::TERRAIN_SIZE);
	m_noiseMapBuilder->SetBounds(0.0, 20.0, 0.0, 20.0);
	m_noiseMapBuilder->Build();
}
