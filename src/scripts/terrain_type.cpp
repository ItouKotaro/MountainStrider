//=============================================================
//
// �n�`�̎�� [terrain_type.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain_type.h"
#include "terrain.h"

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
	m_noiseMapBuilder->SetDestSize(Terrain::TERRAIN_SIZE, Terrain::TERRAIN_SIZE);
	m_noiseMapBuilder->SetBounds(0.0, 20.0, 0.0, 20.0);
	m_noiseMapBuilder->Build();
}
