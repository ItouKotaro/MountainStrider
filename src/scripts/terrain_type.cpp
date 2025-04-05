//=============================================================
//
// �n�`�̎�� [terrain_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain_type.h"
#include "terrain.h"
#include "scene/game.h"

//=============================================================
// [TerrainNoise] �R���X�g���N�^
//=============================================================
TerrainNoise::TerrainNoise() : m_noiseMap(nullptr), m_noiseMapBuilder(nullptr)
{
	// �n�`���擾����
	auto terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();
	m_terrainSize = terrain->GetTerrainSize();
	m_terrainScale = terrain->GetTerrainScale();
}


//=============================================================
// [DesertTerrainNoise] ��������
//=============================================================
void DesertTerrainNoise::Generate()
{
	// �p�[�����m�C�Y�𐶐�����
	module::Perlin myModule;
	myModule.SetSeed(rand());

	// �X�P�[����ύX����
	module::ScaleBias scaled;
	scaled.SetSourceModule(0, myModule);
	scaled.SetScale(100.0f);

	// ��������
	m_noiseMapBuilder->SetSourceModule(scaled);
	m_noiseMapBuilder->SetDestNoiseMap(*m_noiseMap);
	m_noiseMapBuilder->SetDestSize(m_terrainSize, m_terrainSize);
	m_noiseMapBuilder->SetBounds(0.0, 20.0, 0.0, 20.0);
	m_noiseMapBuilder->Build();
}
