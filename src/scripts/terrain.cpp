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
const float CTerrain::TERRAIN_SCALE = 300.0f;

//=============================================================
// [CTerrain] 初期化
//=============================================================
void CTerrain::Init()
{
	// メッシュフィールドを作成する
	m_pField = new GameObject;
	m_pField->SetParent(gameObject);
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);

	m_terrainData = nullptr;

	// 障害物を登録する

	// 木
	CProdTree* prodTree = new CProdTree();
	prodTree->SetChance(10);
	prodTree->SetAdjacentRate("tree", 10.0f);
	RegisterProduces(prodTree);
	
	// フェンス
	CProdFence* prodFence = new CProdFence();
	prodFence->SetChance(0);
	prodFence->SetAdjacentRate("fence", 100.0f);
	RegisterProduces(prodFence);
}

//=============================================================
// [CTerrain] 終了
//=============================================================
void CTerrain::Uninit()
{
	UninitTerrain();

	for (unsigned int i = 0; i < m_natureProduces.size(); i++)
	{
		if (m_natureProduces[i] != nullptr)
		{
			delete m_natureProduces[i];
			m_natureProduces[i] = nullptr;
		}
	}
	m_natureProduces.clear();
}

//=============================================================
// [CTerrain] 地形の終了処理
//=============================================================
void CTerrain::UninitTerrain()
{
	CCollision::RemoveCollision(m_pField);

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
	this->UninitTerrain();

	// コリジョンを作成する
	CCollision::Create(m_pField);

	// 地形データを生成する
	GenerateTerrain();

	// HeightfieldTerrainShapeを作成する
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);

	// 設定したシェープを適用する
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// 生成物を生成する
	srand((unsigned int)clock());
	for (int i = 0; i < 1500; i++)
	{
		GenerateProduces();
	}

}

//=============================================================
// [CTerrain] 地形の生成
//=============================================================
void CTerrain::GenerateTerrain()
{
	// 高度マップを生成する
	dtl::shape::PerlinSolitaryIsland<int>(0.5f, 0.45f, 6.0f, 6, 300, -200).draw(m_terrainHeight);
	//dtl::shape::DiamondSquareAverageIsland<int>(0, 50, 0).draw(m_terrainHeight);

	// 山道を生成する


	// メッシュの高さを変更する
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainHeight[x][y] *= 10.0f;
			m_pField->GetComponent<CMeshField>()->SetHeight(x, y, static_cast<float>(m_terrainHeight[x][y]));
		}
	}

	// テクスチャを生成する
	m_pField->GetComponent<CMeshField>()->SetTexture("data\\TEXTURE\\ground.png");
	m_pField->GetComponent<CMeshField>()->SetLoopTexture(100);
	m_pField->GetComponent<CMeshField>()->ResetNormals();

	// 地形情報を格納する
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = static_cast<float>(m_terrainHeight[x][y]);
		}
	}
}

//=============================================================
// [CTerrain] 山道の生成
//=============================================================
void CTerrain::GenerateRoad()
{

}

//=============================================================
// [CTerrain] 生成物の登録
//=============================================================
void CTerrain::RegisterProduces(CNatureProduces* pNatureProduce)
{
	// もう既に登録されていないかを確認する
	for (unsigned int i = 0; i < m_natureProduces.size(); i++)
	{
		if (m_natureProduces[i] == pNatureProduce)
		{ // 一致するとき
			return;
		}
	}

	// 登録する
	m_natureProduces.push_back(pNatureProduce);
}

//=============================================================
// [CTerrain] 生成物の生成
//=============================================================
void CTerrain::GenerateProduces()
{
	for (int nTryCount = 0; nTryCount < 500; nTryCount++)
	{
		// ランダムで位置を決める
		D3DXVECTOR3 generatePos;
		generatePos = {
			rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f,
			0.0f,
			rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f
		};

		// 生成物を決定する
		CNatureProduces* pSelectProduce = nullptr;
		int nAllChance = 0;		// すべての確率
		for (unsigned int i = 0; i < m_natureProduces.size(); i++)
		{
			nAllChance += static_cast<int>(m_natureProduces[i]->GetChance() * m_natureProduces[i]->GetAdjacentRate(generatePos));
		}
		int nRandValue = rand() % nAllChance;	// ランダム値
		int nMinChance = 0;								// チャンス範囲の最小値
		int nMaxChance = 0;								// チャンス範囲の最大値
		for (unsigned int i = 0; i < m_natureProduces.size(); i++)
		{
			nMaxChance = nMinChance + m_natureProduces[i]->GetChance();
			if (nMinChance <= nRandValue && nRandValue < nMaxChance)
			{ // 範囲内のとき
				pSelectProduce = m_natureProduces[i];
				break;
			}
			nMinChance += m_natureProduces[i]->GetChance();
		}

		if (pSelectProduce == nullptr)
		{ // 生成物が決定しなかったとき（例外）
			continue;
		}

		// レイポイントの位置を決める
		D3DXVECTOR3 rayPoint[4];
		rayPoint[0].x = -pSelectProduce->GetSize().x / 2;
		rayPoint[0].z = -pSelectProduce->GetSize().y / 2;
		rayPoint[1].x = pSelectProduce->GetSize().x / 2;
		rayPoint[1].z = -pSelectProduce->GetSize().y / 2;
		rayPoint[2].x = -pSelectProduce->GetSize().x / 2;
		rayPoint[2].z = pSelectProduce->GetSize().y / 2;
		rayPoint[3].x = pSelectProduce->GetSize().x / 2;
		rayPoint[3].z = pSelectProduce->GetSize().y / 2;

		// 生成座標に移動する
		for (int i = 0; i < 4; i++)
		{
			rayPoint[i] += generatePos;
		}

		// レイを飛ばす
		bool bReached = true;
		for (int i = 0; i < 4; i++)
		{
			btVector3 Start = btVector3(rayPoint[i].x, 3000.0f, rayPoint[i].z);
			btVector3 End = btVector3(rayPoint[i].x, -3000.0f, rayPoint[i].z);

			btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
			CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
			if (RayCallback.hasHit())
			{ // ヒットしたとき
				if (RayCallback.m_collisionObject != CCollision::GetCollision(m_pField)->GetGhostObject())
				{ // 地面に接触しなかったとき
					bReached = false;
					break;
				}
			}
		}

		// すべてのレイが地面に到達したとき
		if (bReached)
		{
			// 中心にレイを飛ばして設置高度を取得する
			btVector3 Start = btVector3(generatePos.x, 3000.0f, generatePos.z);
			btVector3 End = btVector3(generatePos.x, -3000.0f, generatePos.z);

			btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
			CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
			if (RayCallback.hasHit())
			{ // ヒットしたとき
				// ヒットしたYの高さを設置位置にする
				generatePos.y = RayCallback.m_hitPointWorld.getY() + pSelectProduce->GetOffsetY();

				// オブジェクトを設置する
				pSelectProduce->Generate(Transform(generatePos))->SetParent(gameObject);

				// ループを抜ける
				break;
			}
		}
	}
}