//=============================================================
//
// 地形 [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "component/3d/meshfield.h"
#include <DTL.hpp>

// 静的メンバ変数の初期化

//=============================================================
// [CTerrain] 初期化
//=============================================================
void CTerrain::Init()
{
	// メッシュフィールドを作成する
	m_pField = new GameObject;
	m_pField->SetParent(gameObject);
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, 100.0f);

	m_indices = nullptr;
	m_vertices = nullptr;
	m_terrainData = nullptr;
}

//=============================================================
// [CTerrain] 終了
//=============================================================
void CTerrain::Uninit()
{
	CCollision::RemoveCollision(m_pField);
	if (m_indices != nullptr)
	{
		delete[] m_indices;
		m_indices = nullptr;
	}
	if (m_vertices != nullptr)
	{
		delete[] m_vertices;
		m_vertices = nullptr;
	}

	if (m_terrainData != nullptr)
	{
		delete[] m_terrainData;
		m_terrainData = nullptr;
	}
	if (m_terrainShape != nullptr)
	{
		delete m_terrainShape;
		m_terrainShape = nullptr;
	}
}

//=============================================================
// [CTerrain] 生成
//=============================================================
void CTerrain::Generate()
{
	// 終了する
	this->Uninit();

	// コリジョンを作成する
	CCollision::Create(m_pField);

	// 高度マップを生成する
	dtl::shape::PerlinSolitaryIsland<int>(0.5f, 0.45f, 6.0f, 6, 300, -200).draw(m_terrainHeight);
	//dtl::shape::DiamondSquareAverageIsland<int>(0, 50, 0).draw(m_terrainHeight);

	// メッシュの高さを変更する
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_pField->GetComponent<CMeshField>()->SetHeight(x, y, m_terrainHeight[x][y]);
		}
	}

	// テクスチャを生成する
	m_pField->GetComponent<CMeshField>()->SetTexture("data\\TEXTURE\\ground.png");
	m_pField->GetComponent<CMeshField>()->SetLoopTexture(10);

	// 地形情報を格納する
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE] ;
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = static_cast<float>(m_terrainHeight[x][y]);
		}
	}

	// HeightfieldTerrainShapeを作成する
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -1000, 1000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(100.0f, 1.0f, 100.0f));
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);
}