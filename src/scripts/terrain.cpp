//=============================================================
//
// 地形 [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "component/3d/meshfield.h"
using namespace noise;

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

	// 高度カラーを設定する
	AddHeightColor(120.0f, D3DCOLOR_RGBA(56, 201, 93, 255));
	AddHeightColor(-120.0f, D3DCOLOR_RGBA(5, 97, 29, 255));
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
	for (int i = 0; i < 2000; i++)
	{
		GenerateProduces();
	}

}

//=============================================================
// [CTerrain] 地形の生成
//=============================================================
void CTerrain::GenerateTerrain()
{
	module::Perlin myModule;
	utils::NoiseMap heightMap;

	myModule.SetSeed((unsigned int)clock());

	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	renderer.AddGradientPoint(-1.0000, utils::Color(0, 0, 128, 255)); // deeps
	renderer.AddGradientPoint(-0.2500, utils::Color(0, 0, 255, 255)); // shallow
	renderer.AddGradientPoint(0.0000, utils::Color(0, 128, 255, 255)); // shore
	renderer.AddGradientPoint(0.0625, utils::Color(240, 240, 64, 255)); // sand
	renderer.AddGradientPoint(0.1250, utils::Color(32, 160, 0, 255)); // grass
	renderer.AddGradientPoint(0.3750, utils::Color(224, 224, 0, 255)); // dirt
	renderer.AddGradientPoint(0.7500, utils::Color(128, 128, 128, 255)); // rock
	renderer.AddGradientPoint(1.0000, utils::Color(255, 255, 255, 255)); // snow
	renderer.EnableLight();
	renderer.SetLightContrast(3.0);
	renderer.SetLightBrightness(2.0); // Double the brightness
	renderer.Render();

	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("tutorial.bmp");
	writer.WriteDestFile();

	// 山道を生成する
	GenerateRoad();

	// 地形情報を格納する
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y) * 1000.0f;
		}
	}

	// メッシュ情報を変更する
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			float height = m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE];

			// 高さを設定する
			m_pField->GetComponent<CMeshField>()->SetHeight(x, y, height);

			// 色を設定する
			m_pField->GetComponent<CMeshField>()->SetColor(x, y, GetHeightColor(height));

		}
	}

	// テクスチャを生成する
	//m_pField->GetComponent<CMeshField>()->SetTexture("data\\TEXTURE\\ground.png");
	//m_pField->GetComponent<CMeshField>()->SetLoopTexture(100);

	m_pField->GetComponent<CMeshField>()->ResetNormals();
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

//=============================================================
// [CTerrain] 高度の色を追加する
//=============================================================
void CTerrain::AddHeightColor(const float& height, const D3DXCOLOR& color)
{
	// データを入れる
	HeightColor heightColor;
	heightColor.height = height;
	heightColor.color = color;
	m_heightColor.push_back(heightColor);
}

//=============================================================
// [CTerrain] 高度の色を取得する
//=============================================================
D3DXCOLOR CTerrain::GetHeightColor(const float& height)
{
	if (m_heightColor.size() <= 0)
	{ // 高度カラーが無いときは白を返す
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 高い 色を取得
	D3DXCOLOR highColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float highHeight = 0.0f;
	bool foundHighHeight = false;
	for (unsigned int i = 0; i < m_heightColor.size(); i++)
	{
		if (height < m_heightColor[i].height &&
			(!foundHighHeight | highHeight > m_heightColor[i].height))
		{
			highColor = m_heightColor[i].color;
			highHeight = m_heightColor[i].height;
			foundHighHeight = true;
		}
	}

	// 低い 色を取得
	D3DXCOLOR lowColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	float lowHeight = 0.0f;
	bool foundLowHeight = false;
	for (unsigned int i = 0; i < m_heightColor.size(); i++)
	{
		if (height > m_heightColor[i].height &&
			(!foundLowHeight | lowHeight < m_heightColor[i].height))
		{
			lowColor = m_heightColor[i].color;
			lowHeight = m_heightColor[i].height;
			foundLowHeight = true;
		}
	}

	// 両方見つからなかったときの処理
	if (!foundHighHeight & !foundLowHeight)
	{
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// 片方のみ見つかったときの処理
	if (foundHighHeight ^ foundLowHeight)
	{
		return foundHighHeight ? highColor : lowColor;
	}

	// 高さの範囲から現在位置の割合を計算する
	float currentPercent = (height - lowHeight) / (highHeight - lowHeight);

	// 割合から色を計算する
	D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	color = highColor * currentPercent + lowColor * (1.0f - currentPercent);

	return color;
}