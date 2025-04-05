//=============================================================
//
// 地形の種類 [terrain_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain_type.h"
#include "terrain.h"
#include "scene/game.h"

//=============================================================
// [TerrainNoise] コンストラクタ
//=============================================================
TerrainNoise::TerrainNoise() : m_noiseMap(nullptr), m_noiseMapBuilder(nullptr)
{
	// 地形を取得する
	auto terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();
	m_terrainSize = terrain->GetTerrainSize();
	m_terrainScale = terrain->GetTerrainScale();
}


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
	m_noiseMapBuilder->SetDestSize(m_terrainSize, m_terrainSize);
	m_noiseMapBuilder->SetBounds(0.0, 20.0, 0.0, 20.0);
	m_noiseMapBuilder->Build();
}
