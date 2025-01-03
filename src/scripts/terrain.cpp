//=============================================================
//
// 地形 [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "benlib.h"
#include <fstream>
#include "manager.h"

#include "component/3d/meshfield.h"
#include "scripts/vehicle.h"
#include "scripts/wreckage.h"
#include "component/3d/collision.h"
#include "component/3d/field.h"
#include "scripts/road.h"
#include "scripts/gem.h"
using namespace noise;

#include "component/2d/text.h"
#include "renderer.h"

// 静的メンバ変数の初期化
const float Terrain::TERRAIN_SCALE = 150.0f;

//=============================================================
// [Terrain] 初期化
//=============================================================
void Terrain::Init()
{
	m_pShadowField = new GameObject("ShadowTerrain");
	m_pShadowField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);
	m_pShadowField->transform->SetPos(0.0f, -10.0f);

	// メッシュフィールドを作成する
	m_pField = new GameObject;
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);
	m_pField->AddComponent<Road>();

	m_terrainData = nullptr;
	m_producesManager = new ProducesManager();
	m_producesManager->Init();

	// 障害物を登録する
	LoadTerrainFile("data\\TERRAINS\\terrains.json");
}

//=============================================================
// [Terrain] 終了
//=============================================================
void Terrain::Uninit()
{
	// 地形の破棄
	UninitTerrain();

	// 登録生成物を解放する
	for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
	{
		if (m_registerNatureProduces[i] != nullptr)
		{
			delete m_registerNatureProduces[i];
			m_registerNatureProduces[i] = nullptr;
		}
	}
	m_registerNatureProduces.clear();

	// 生成物管理を解放する
	if (m_producesManager != nullptr)
	{
		m_producesManager->Uninit();
		delete m_producesManager;
		m_producesManager = nullptr;
	}
}

//=============================================================
// [Terrain] 更新
//=============================================================
void Terrain::Update(const D3DXVECTOR3& pos)
{
	// 生成物を更新する
	m_producesManager->Update(pos);
}

//=============================================================
// [Terrain] 地形の終了処理
//=============================================================
void Terrain::UninitTerrain()
{
	CCollision::RemoveCollision(m_pField);
	CCollision::RemoveCollision(m_pShadowField);

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
// [Terrain] 生成
//=============================================================
void Terrain::Generate()
{
	// 終了する
	this->UninitTerrain();

	// コリジョンを作成する
	CCollision::Create(m_pField);
	CCollision::Create(m_pShadowField);

	// シード値を設定する
	srand(m_seed);

	// 地形データを生成する
	GenerateTerrain();

	m_pField->GetComponent<CMeshField>()->SetTexture("data\\terrain.bmp");

	// HeightfieldTerrainShapeを作成する
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_pField)->SetFriction(100.0f);
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);
	CCollision::GetCollision(m_pShadowField)->GetGhostObject()->setCollisionShape(m_terrainShape);

	// 設定したシェープを適用する
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// 道のオブジェクトを生成する
	m_pField->GetComponent<Road>()->Generate();

	// ジェムの生成
	for (int i = 0; i < 100; i++)
	{
		GenerateGem();
	}

	// 生成物を生成する
	for (int i = 0; i < 20000; i++)
	{
		GenerateProduces();
	}

}

//=============================================================
// [Terrain] 地形の生成
//=============================================================
void Terrain::GenerateTerrain()
{
	module::Perlin myModule;
	utils::NoiseMap heightMap;

	// シードを設定する
	myModule.SetSeed(rand());

	// 地形を生成する
	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 6.0);
	heightMapBuilder.Build();

	//for (int x = 0; x < TERRAIN_SIZE; x++)
	//{
	//	for (int y = 0; y < TERRAIN_SIZE; y++)
	//	{
	//		heightMap.SetValue(x, y, heightMap.GetValue(x, y) * 1300.0f);
	//	}
	//}

	// 画像に書き出す
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	for (auto itr = m_heightColor.begin(); itr != m_heightColor.end(); itr++)
	{
		renderer.AddGradientPoint((*itr).height, utils::Color(static_cast<noise::uint8>((*itr).color.r * 255), static_cast<noise::uint8>((*itr).color.g * 255), static_cast<noise::uint8>((*itr).color.b * 255), static_cast<noise::uint8>((*itr).color.a * 255)));
	}
	renderer.EnableLight();
	renderer.SetLightContrast(3.0);
	renderer.SetLightBrightness(1.0);
	renderer.Render();

	// ファイルに書き出す
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("data\\terrain.bmp");
	writer.WriteDestFile();


	// 地形情報を格納する
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y) * 450.0f;
		}
	}

	// 最低と最高高度を取得する
	m_minHeight = GetVertexHeight(0, 0);
	m_maxHeight = GetVertexHeight(0, 0);
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			float value = GetVertexHeight(x, y);
			if (value < m_minHeight)
			{
				m_minHeight = value;
			}
			if (value > m_maxHeight)
			{
				m_maxHeight = value;
			}
		}
	}

	// 道を生成する
	GenerateRoad();

	// メッシュ情報を変更する
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			float height = GetVertexHeight(x, y);

			// 高さを設定する
			m_pField->GetComponent<CMeshField>()->SetHeight(x, y, height);
			m_pShadowField->GetComponent<CMeshField>()->SetHeight(x, y, height);
		}
	}

	// 法線をリセットする
	m_pField->GetComponent<CMeshField>()->ResetNormals();
}

//=============================================================
// [Terrain] 道の生成
//=============================================================
void Terrain::GenerateRoad()
{
	// ポイント ------------------------------------------------

	// 道のコンポーネントを取得する
	auto pRoad = m_pField->GetComponent<Road>();

	// 道を生成する
	int routeData[TERRAIN_SIZE][TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			routeData[x][y] = 0;
		}
	}

	// 周辺の高さ情報
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			float centerHeight = GetVertexHeight(x, y);
			for (int sX = 0; sX < 3; sX++)
			{
				for (int sY = 0; sY < 3; sY++)
				{
					if (!(sX == 1 && sY == 1))
					{ // 中心以外
						float height = GetVertexHeight(x - 1 + sX, y - 1 + sY);
						
						// 高低差を取得する
						float heightDifference = height - centerHeight;

						if (heightDifference > 0)
						{ // 高いとき
							routeData[x][y] += static_cast<int>(fabsf(heightDifference) * 0.1f);
						}
						if (fabsf(heightDifference) < 100.0f)
						{ // 低いとき
							routeData[x][y] += static_cast<int>(100.0f - fabsf(heightDifference));
						}
					}
				}
			}
		}
	}

	// 中心によるほどポイントが高くなる
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			int dX = abs(x) <= abs(x - TERRAIN_SIZE) ? abs(x) : abs(x - TERRAIN_SIZE);
			int dY = abs(y) <= abs(y - TERRAIN_SIZE) ? abs(y) : abs(y - TERRAIN_SIZE);
			int disNear = dX <= dY ? dX : dY;

			routeData[x][y] += static_cast<int>(disNear * 25);
		}
	}

	// 道選択 ------------------------------------------------
	for (int i = 0; i < 2; i++)
	{
		pRoad->AddIdx();

		bool isAxisX = rand() % 2 == 0;
		int currentX = isAxisX ? (rand() % TERRAIN_SIZE) : (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1);
		int currentY = isAxisX ? (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1) : (rand() % TERRAIN_SIZE);

		// 最終地点を設定する（縁）
		Area startArea = NONE;
		while (1)
		{
			isAxisX = rand() % 2 == 0;
			currentX = isAxisX ? (rand() % TERRAIN_SIZE) : (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1);
			currentY = isAxisX ? (rand() % 2 == 0 ? 0 : TERRAIN_SIZE - 1) : (rand() % TERRAIN_SIZE);

			// 辺を割り出す
			if (currentX == 0) startArea = LEFT;
			if (currentX == TERRAIN_SIZE - 1) startArea = RIGHT;
			if (currentY == 0) startArea = TOP;
			if (currentY == TERRAIN_SIZE - 1) startArea = BOTTOM;

			if (m_startArea != startArea)
			{
				m_startArea = startArea;
				break;
			}
		}
		pRoad->AddPoint(i, currentX, currentY);

		// 道を伸ばす
		for (int pass = 0; pass < 200; pass++)
		{
			// 現在地点からポイントの高い方向に向かう
			int selectX = 0;
			int selectY = 0;
			int high = 0;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// 除外リスト
					if ((x == 0 && y == 0) ||
						currentX + x < 0 || TERRAIN_SIZE <= currentX + x ||
						currentY + y < 0 || TERRAIN_SIZE <= currentY + y)
					{ // 現在位置と範囲外
						continue;	// スキップ
					}

					// 既にルートの時も除外
					if (pRoad->IsRoutedByIdx(i, currentX + x, currentY + y))
					{
						continue;	// スキップ
					}

					// 周囲に道があるとき
					int nearRoad = 0;
					for (int sx = -1; sx < 2; sx++)
					{
						for (int sy = -1; sy < 2; sy++)
						{
							// 除外リスト
							if ((sx == 0 && sy == 0) ||
								x + currentX + sx < 0 || TERRAIN_SIZE <= x + currentX + sx ||
								y + currentY + sy < 0 || TERRAIN_SIZE <= y + currentY + sy)
							{ // 現在位置と範囲外
								continue;	// スキップ
							}

							// 既に道の場合
							if (pRoad->IsRoutedByIdx(i, x + currentX + sx, y + currentY + sy))
							{
								nearRoad++;
							}
						}
					}
					if (nearRoad > 1)
					{
						routeData[currentX + x][currentY + y] *= 0.2f;
					}

					if (routeData[currentX + x][currentY + y] > high)
					{ // 最高スコアの時
						selectX = currentX + x;
						selectY = currentY + y;
						high = routeData[currentX + x][currentY + y];
					}
				}
			}
			currentX = selectX;
			currentY = selectY;

			bool isEnded = false;
			// 終了条件: 中心にたどり着く
			if (currentX == TERRAIN_SIZE / 2 && currentY == TERRAIN_SIZE / 2)
			{
				isEnded = true;
			}

			// 終了条件: 他の道と衝突
			int routedIdx = pRoad->IsIndexRouted(currentX, currentY);
			if (routedIdx != -1 && routedIdx != i)
			{
				isEnded = true;
			}

			// 終了条件: 別の辺に衝突
			Area currentArea = NONE;
			if (currentX == 0) currentArea = LEFT;
			if (currentX == TERRAIN_SIZE - 1) currentArea = RIGHT;
			if (currentY == 0) currentArea = TOP;
			if (currentY == TERRAIN_SIZE - 1) currentArea = BOTTOM;
			if (currentArea != NONE && startArea != currentArea)
			{
				isEnded = true;
			}

			// 道を追加する
			pRoad->AddPoint(i, currentX, currentY);

			// 道を平地化する
			int aveCount = 0;
			float ave = 0.0f;
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// 除外リスト
					if (currentX + x < 0 || TERRAIN_SIZE <= currentX + x ||
						currentY + y < 0 || TERRAIN_SIZE <= currentY + y)
					{ // 現在位置と範囲外
						continue;	// スキップ
					}
					ave += GetVertexHeight(currentX + x, currentY + y);
					aveCount++;
				}
			}
			// 平均値を出す
			ave /= (float)aveCount;
			// 設定する
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// 除外リスト
					if (currentX + x < 0 || TERRAIN_SIZE <= currentX + x ||
						currentY + y < 0 || TERRAIN_SIZE <= currentY + y)
					{ // 現在位置と範囲外
						continue;	// スキップ
					}
					float ori = (GetVertexHeight(currentX + x, currentY + y) - ave) * 0.5f;
					SetVertexHeight(currentX + x, currentY + y, ave + ori);
				}
			}


			if (isEnded)
				break;
		}
	}

	// 生成する
	pRoad->OutputText();
}

//=============================================================
// [Terrain] ジェムの生成
//=============================================================
void Terrain::GenerateGem()
{
	// ランダムで位置を決める
	D3DXVECTOR3 generatePos;
	generatePos = {
		rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f,
		0.0f,
		rand() % static_cast<int>(TERRAIN_SIZE * TERRAIN_SCALE) - TERRAIN_SIZE * TERRAIN_SCALE * 0.5f
	};

	btVector3 Start = btVector3(generatePos.x, m_maxHeight + 10.0f, generatePos.z);
	btVector3 End = btVector3(generatePos.x, m_minHeight - 10.0f, generatePos.z);

	btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
	CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
	if (RayCallback.hasHit())
	{ // ヒットしたとき
		GameObject* gemObj = GameObject::LoadPrefab("data\\PREFAB\\item\\gem.pref", Transform({generatePos.x, RayCallback.m_hitPointWorld.getY() + 20.0f, generatePos.z}));
		gemObj->AddComponent<Gem>();
	}
}

//=============================================================
// [Terrain] 頂点の高さを取得する
//=============================================================
float Terrain::GetVertexHeight(const int& x, const int& y)
{
	if (0 <= x && x < TERRAIN_SIZE &&
		0 <= y && y < TERRAIN_SIZE)
	{
		return m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE];
	}
	return 0.0f;
}

//=============================================================
// [Terrain] 頂点の高さを設定する
//=============================================================
void Terrain::SetVertexHeight(const int& x, const int& y, const float& height)
{
	if (0 <= x && x < TERRAIN_SIZE &&
		0 <= y && y < TERRAIN_SIZE)
	{
		m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = height;
	}
}

//=============================================================
// [Terrain] 生成物の登録
//=============================================================
void Terrain::RegisterProduces(CNatureProduces* pNatureProduce)
{
	// もう既に登録されていないかを確認する
	for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
	{
		if (m_registerNatureProduces[i] == pNatureProduce)
		{ // 一致するとき
			return;
		}
	}

	// 登録する
	m_registerNatureProduces.push_back(pNatureProduce);
}

//=============================================================
// [Terrain] 生成物の生成
//=============================================================
void Terrain::GenerateProduces()
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

		// 道の近くのとき
		int chunkX = static_cast<int>((generatePos.x + TERRAIN_SIZE * TERRAIN_SCALE * 0.5f) / (float)TERRAIN_SCALE);
		int chunkY = static_cast<int>((generatePos.y + TERRAIN_SIZE * TERRAIN_SCALE * 0.5f) / (float)TERRAIN_SCALE);
		if (m_pField->GetComponent<Road>()->IsRouted(chunkX, chunkY))
		{ // すでに道のとき
			continue;
		}

		// 生成物を決定する
		CNatureProduces* pSelectProduce = nullptr;
		int nAllChance = 0;		// すべての確率
		for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
		{
			nAllChance += static_cast<int>(m_registerNatureProduces[i]->GetChance() * m_registerNatureProduces[i]->GetAdjacentRate(generatePos));
		}
		int nRandValue = rand() % nAllChance;	// ランダム値
		int nMinChance = 0;								// チャンス範囲の最小値
		int nMaxChance = 0;								// チャンス範囲の最大値
		for (unsigned int i = 0; i < m_registerNatureProduces.size(); i++)
		{
			nMaxChance = nMinChance + m_registerNatureProduces[i]->GetChance();
			if (nMinChance <= nRandValue && nRandValue < nMaxChance)
			{ // 範囲内のとき
				pSelectProduce = m_registerNatureProduces[i];
				break;
			}
			nMinChance += m_registerNatureProduces[i]->GetChance();
		}

		if (pSelectProduce == nullptr)
		{ // 生成物が決定しなかったとき（例外）
			continue;
		}

		// 周辺に生成物がないかを確認する
		if (m_producesManager->FindProducesByDistance(generatePos, pSelectProduce->GetSize()))
		{ // 存在するとき
			continue;
		}

		// レイポイントの位置を決める
		D3DXVECTOR3 rayPoint[3];
		rayPoint[0].x = -pSelectProduce->GetSize() / 2;
		rayPoint[0].z = pSelectProduce->GetSize() / 2;
		rayPoint[1].x = pSelectProduce->GetSize() / 2;
		rayPoint[1].z = pSelectProduce->GetSize() / 2;
		rayPoint[2].x = 0.0f;
		rayPoint[2].z = -pSelectProduce->GetSize() / 2;

		// 生成座標に移動する
		for (int i = 0; i < 3; i++)
		{
			rayPoint[i] += generatePos;
		}

		// レイを飛ばす
		D3DXVECTOR3 rayReachPoint[3];
		bool bReached = true;
		for (int i = 0; i < 3; i++)
		{
			btVector3 Start = btVector3(rayPoint[i].x, m_maxHeight + 10.0f, rayPoint[i].z);
			btVector3 End = btVector3(rayPoint[i].x, m_minHeight - 10.0f, rayPoint[i].z);

			btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
			CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
			if (RayCallback.hasHit())
			{ // ヒットしたとき
				if (RayCallback.m_collisionObject != CCollision::GetCollision(m_pField)->GetGhostObject())
				{ // 地面に接触しなかったとき
					bReached = false;
					break;
				}
				rayReachPoint[i] = { RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ()};
			}
			else
			{
				bReached = false;
				break;
			}
		}

		// 法線ベクトルを計算する
		D3DXVECTOR3 normal = Benlib::CalcNormalVector(rayReachPoint[0], rayReachPoint[1], rayReachPoint[2]);

		// 傾斜制限の条件に満たしていないとき
		if (!(fabsf(atan2f(normal.z, normal.y)) >= pSelectProduce->GetSlantAngleLimitMin() &&
			fabsf(atan2f(normal.z, normal.y)) <= pSelectProduce->GetSlantAngleLimitMax()))
		{
			continue;
		}

		// すべてのレイが地面に到達したとき
		if (bReached)
		{
			D3DXQUATERNION rot;
			D3DXQuaternionIdentity(&rot);

			// ランダム角度を取得する
			float angleRange = pSelectProduce->GetAngleRange();
			std::function<float()> randAngle = [angleRange]() {
				if (angleRange == 0)
					return 0.0f;
				return (rand() % static_cast<int>(angleRange * 1000) - static_cast<int>(angleRange * 500)) * 0.001f; 
			};

			D3DXVECTOR3 axis = { 0.0f, 0.0f, 0.0f };
			if (!pSelectProduce->IsMatchInclination())
			{ // 傾斜角を考慮しない
				D3DXQuaternionRotationYawPitchRoll(&rot, randAngle(), 0.0f, randAngle());
			}
			else
			{ // 傾斜角を考慮する
				// 地形に合わせた角度を計算する
				axis = { 1.0f, 0.0f, 0.0f };
				D3DXQuaternionRotationAxis(&rot, &axis, fabsf(atan2f(normal.z, normal.y)) + randAngle());

				D3DXQUATERNION mulRot;
				axis = { 0.0f, 1.0f, 0.0f };
				D3DXQuaternionRotationAxis(&mulRot, &axis, atan2f(normal.x, normal.z) + randAngle());
				rot *= mulRot;
			}

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
				Transform trans;
				trans.SetPos(generatePos);
				trans.SetQuaternion(rot);
				m_producesManager->AddProduce(trans, pSelectProduce);

				// ループを抜ける
				break;
			}
		}
	}
}

//=============================================================
// [Terrain] 高度の色を追加する
//=============================================================
void Terrain::AddHeightColor(const float& height, const D3DXCOLOR& color)
{
	// データを入れる
	HeightColor heightColor;
	heightColor.height = height;
	heightColor.color = color;
	m_heightColor.push_back(heightColor);
}

//=============================================================
// [Terrain] 傾斜の色を追加する
//=============================================================
void Terrain::AddSlantColor(const float& minHeight, const float& maxHeight, const D3DXCOLOR& color, const float& rate)
{
	// データを入れる
	SlantColor slantColor;
	slantColor.minHeight = minHeight;
	slantColor.maxHeight = maxHeight;
	slantColor.color = color;
	slantColor.rate = rate;
	m_slantColor.push_back(slantColor);
}

//=============================================================
// [Terrain] 高度の色を取得する
//=============================================================
D3DXCOLOR Terrain::GetHeightColor(const float& height)
{
	if (m_heightColor.size() <= 0)
	{ // 高度カラーが無いときは白を返す
		return D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	}

	float heightNormalize = ((height - m_minHeight) / static_cast<float>(m_maxHeight - m_minHeight) * 2) - 1.0f;

	// 高い 色を取得
	D3DXCOLOR highColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float highHeight = 0.0f;
	bool foundHighHeight = false;
	for (unsigned int i = 0; i < m_heightColor.size(); i++)
	{
		if (heightNormalize < m_heightColor[i].height &&
			(!foundHighHeight || highHeight > m_heightColor[i].height))
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
		if (heightNormalize > m_heightColor[i].height &&
			(!foundLowHeight || lowHeight < m_heightColor[i].height))
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
	float currentPercent = (heightNormalize - lowHeight) / (highHeight - lowHeight);

	// 割合から色を計算する
	D3DXCOLOR color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	color = highColor * currentPercent + lowColor * (1.0f - currentPercent);

	return color;
}

//=============================================================
// [Terrain] 高度の色を取得する
//=============================================================
D3DXCOLOR Terrain::GetVertexColor(const int& x, const int& y)
{
	// 高度を取得する
	float height = GetVertexHeight(x, y);

	// 指定位置からの最大高度を求める
	float positiveHeight = 0.0f;
	for (int vx = -1; vx < 2; vx++)
	{
		for (int vy = -1; vy < 2; vy++)
		{
			if (0 <= x + vx && x + vx < TERRAIN_SIZE &&
				0 <= y + vy && y + vy < TERRAIN_SIZE)
			{
				positiveHeight = positiveHeight < fabsf(height - GetVertexHeight(x + vx, y + vy)) ? fabsf(height - GetVertexHeight(x + vx, y + vy)) : positiveHeight;
			}
		}
	}

	// 範囲内の傾斜カラーを取得する
	D3DXCOLOR slantColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float slantRate = 0.0f;
	for (unsigned int i = 0; i < m_slantColor.size(); i++)
	{
		if (m_slantColor[i].minHeight <= positiveHeight &&
			m_slantColor[i].maxHeight >= positiveHeight)
		{ // 範囲内が見つかったとき
			slantColor = m_slantColor[i].color;
			slantRate = m_slantColor[i].rate;
			break;
		}
	}

	// 結果の色を計算する
	D3DXCOLOR result = GetHeightColor(height) * (1.0f - slantRate) + slantColor * slantRate;
	return result;
}

//=============================================================
// [Terrain] 地形ファイルの読み込み
//=============================================================
void Terrain::LoadTerrainFile(const std::string path)
{
	// jsonファイルを読み込む
	std::ifstream ifs(path.c_str());

	if (ifs.fail())
	{ // ファイルの読み込みに失敗
		MessageBox(CManager::GetInstance()->GetHWND(), "地形情報ファイルの読み込みに失敗しました", "エラー", MB_OK); //終了メッセージ
		return;
	}

	// json形式に変換
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jInput = json::parse(sInputData);

	// 地形情報を読み込む
	if (jInput.contains("terrain"))
	{
		// 高度カラー
		if (jInput["terrain"].contains("height_color"))
		{
			for (auto itr = jInput["terrain"]["height_color"].begin(); itr != jInput["terrain"]["height_color"].end(); itr++)
			{
				if ((*itr).contains("color") && (*itr).contains("height"))
				{
					AddHeightColor((*itr)["height"], D3DCOLOR_RGBA((*itr)["color"][0], (*itr)["color"][1], (*itr)["color"][2], (*itr)["color"][3]));
				}
			}
		}

		// 傾斜カラー
		if (jInput["terrain"].contains("slant_color"))
		{
			for (auto itr = jInput["terrain"]["slant_color"].begin(); itr != jInput["terrain"]["slant_color"].end(); itr++)
			{
				if ((*itr).contains("color") && (*itr).contains("min") && (*itr).contains("max"))
				{
					AddSlantColor((*itr)["min"], (*itr)["max"], D3DCOLOR_RGBA((*itr)["color"][0], (*itr)["color"][1], (*itr)["color"][2], (*itr)["color"][3]));
				}
			}
		}
	}

	// 生成物情報を読み込む
	if (jInput.contains("produces"))
	{ // 生成物の項目があるとき
		for (auto itr = jInput["produces"].begin(); itr != jInput["produces"].end(); itr++)
		{
			CNatureProduces* produce = new CNatureProduces();

			// 名前
			if ((*itr).contains("name"))
			{
				produce->SetName((*itr)["name"]);
			}

			// パス
			if ((*itr).contains("path"))
			{
				produce->SetPath((*itr)["path"]);
			}

			// サイズ
			if ((*itr).contains("size"))
			{
				produce->SetSize((*itr)["size"]);
			}

			// ダメージ
			if ((*itr).contains("damage"))
			{
				produce->SetSize((*itr)["damage"]);
			}

			// オフセットY
			if ((*itr).contains("offset"))
			{
				produce->SetOffsetY((*itr)["offset"]);
			}

			// ランダム角度
			if ((*itr).contains("angle_range"))
			{
				produce->SetAngleRange((*itr)["angle_range"]);
			}

			// 傾斜角に合わせるか
			if ((*itr).contains("match_inclination"))
			{
				produce->SetIsMatchInclination((*itr)["match_inclination"]);
			}

			// 隣接の範囲
			if ((*itr).contains("adjacent_distance"))
			{
				produce->SetAdjacentDistance((*itr)["adjacent_distance"]);
			}

			// 確率
			if ((*itr).contains("chance"))
			{
				produce->SetChance((*itr)["chance"]);
			}

			// 傾斜角度条件
			if ((*itr).contains("limit_slant"))
			{
				produce->SetSlantAngleLimit((*itr)["limit_slant"][0], (*itr)["limit_slant"][1]);
			}

			// 隣接情報
			if ((*itr).contains("adjacent_rate"))
			{ // 隣接情報が含まれているとき
				for (auto adjItr = (*itr)["adjacent_rate"].begin(); adjItr != (*itr)["adjacent_rate"].end(); adjItr++)
				{
					if ((*adjItr).contains("name") && (*adjItr).contains("rate"))
					{
						produce->SetAdjacentRate((*adjItr)["name"], (*adjItr)["rate"]);
					}
				}
			}

			// 生成物を登録する
			RegisterProduces(produce);
		}
	}
}

//=============================================================
// [ProducesManager] 生成物の配置情報を追加する
//=============================================================
void ProducesManager::AddProduce(const Transform& transform, CNatureProduces* pNatureProduce)
{
	// 管理データに追加する
	ManagedProduce* managedProduce = new ManagedProduce;
	managedProduce->transform = transform;
	managedProduce->natureProduce = pNatureProduce;
	managedProduce->managedGameObject = nullptr;
	m_managedProduces.push_back(managedProduce);
}

//=============================================================
// [ProducesManager] 初期化
//=============================================================
void ProducesManager::Init()
{
}

//=============================================================
// [ProducesManager] 終了
//=============================================================
void ProducesManager::Uninit()
{
	// 生成したゲームオブジェクトを破棄する
	for (unsigned int i = 0; i < m_managedGameObjects.size(); i++)
	{
		if (m_managedGameObjects[i] != nullptr)
		{
			m_managedGameObjects[i]->gameObject->Destroy();
			delete m_managedGameObjects[i];
			m_managedGameObjects[i] = nullptr;
		}
	}

	for (unsigned int i = 0; i < m_managedProduces.size(); i++)
	{
		if (m_managedProduces[i] != nullptr)
		{
			delete m_managedProduces[i];
			m_managedProduces[i] = nullptr;
		}
	}
}

//=============================================================
// [ProducesManager] 更新
//=============================================================
void ProducesManager::Update(const D3DXVECTOR3& pos)
{
	// バイクの取得
	if (m_pVehicle == nullptr)
	{
		m_pVehicle = GameObject::Find("Vehicle");
	}

	// ゲームオブジェクトの更新
	UpdateGameObjects(pos);
}

//=============================================================
// [ProducesManager] ゲームオブジェクトの更新
//=============================================================
void ProducesManager::UpdateGameObjects(const D3DXVECTOR3& pos)
{
	for (auto itrManagedProduces = m_managedProduces.begin(); itrManagedProduces != m_managedProduces.end(); itrManagedProduces++)
	{
		if (Benlib::PosDistance(pos, (*itrManagedProduces)->transform.GetWPos()) < 1500.0f)
		{ // 生成範囲内のとき

			if ((*itrManagedProduces)->managedGameObject != nullptr)
			{ // すでに設置済みのとき
				// 破棄カウンターを更新する
				(*itrManagedProduces)->managedGameObject->destroyCounter = 600;

				// バイクとの衝突
				std::vector<GameObject*>& pOverlappingObj = CCollision::GetCollision((*itrManagedProduces)->managedGameObject->gameObject)->GetOverlappingGameObjects();
				for (auto itrOverlappingObj = pOverlappingObj.begin(); itrOverlappingObj != pOverlappingObj.end(); itrOverlappingObj++)
				{
					if (m_pVehicle == *itrOverlappingObj)
					{ // バイクと衝突したとき
						// オブジェクトを破棄する
						ManagedGameObject* pManagedGameObj = nullptr;
						for (auto itrManagedGameObj = m_managedGameObjects.begin(); itrManagedGameObj != m_managedGameObjects.end(); itrManagedGameObj++)
						{
							if (*itrManagedGameObj == (*itrManagedProduces)->managedGameObject)
							{
								// ゲームオブジェクトの管理を破棄する
								pManagedGameObj = *itrManagedGameObj;
								SAFE_ERASE(m_managedGameObjects, itrManagedGameObj)
								break;
							}
						}

						// ゲームオブジェクトと配置情報を破棄
						(*itrManagedProduces)->managedGameObject->gameObject->AddComponent<CRigidBody>();
						(*itrManagedProduces)->managedGameObject->gameObject->AddComponent<CWreckage>();

						// ヒットイベントを実行
						(*itrManagedProduces)->natureProduce->onHit((*itrManagedProduces)->managedGameObject->gameObject);

						// バイクにダメージを与える
						m_pVehicle->GetComponent<CVehicle>()->AddDamage((*itrManagedProduces)->natureProduce->GetDamage());

						// 破棄
						delete pManagedGameObj;
						delete (*itrManagedProduces);
						itrManagedProduces = m_managedProduces.erase(itrManagedProduces);
						break;
					}
				}
				continue;
			}

			// オブジェクトが余っていないか
			bool findVacancy = false;
			for (auto itrManagedGameObj = m_managedGameObjects.begin(); itrManagedGameObj != m_managedGameObjects.end(); itrManagedGameObj++)
			{
				if (!(*itrManagedGameObj)->gameObject->GetActive() &&
					(*itrManagedProduces)->natureProduce == (*itrManagedGameObj)->natureProduce)
				{ // 同じ種類の生成物で非アクティブのとき
					// ゲームオブジェクトのトランスフォームを適用する
					(*itrManagedGameObj)->gameObject->transform->SetPos((*itrManagedProduces)->transform.GetPos());
					(*itrManagedGameObj)->gameObject->transform->SetQuaternion((*itrManagedProduces)->transform.GetQuaternion());
					(*itrManagedGameObj)->gameObject->transform->SetScale((*itrManagedProduces)->transform.GetScale());

					// ゲームオブジェクトをアクティブにする
					(*itrManagedGameObj)->gameObject->SetActive(true);
					(*itrManagedGameObj)->destroyCounter = 600;

					// 設置情報にゲームオブジェクト情報を設定する
					(*itrManagedProduces)->managedGameObject = (*itrManagedGameObj);

					findVacancy = true;
					break;
				}
			}
			if (findVacancy)
			{ // 見つかったとき
				continue;	// 次へ
			}

			// 余りが見つからなかったとき（新規作成）
			ManagedGameObject* managedGameObject = new ManagedGameObject;
			managedGameObject->gameObject = (*itrManagedProduces)->natureProduce->Generate((*itrManagedProduces)->transform);
			managedGameObject->natureProduce = (*itrManagedProduces)->natureProduce;
			managedGameObject->destroyCounter = 600;
			if (managedGameObject->gameObject == nullptr || managedGameObject->natureProduce == nullptr)
			{
				continue;
			}

			// コリジョンを変更する
			managedGameObject->gameObject->Destroy(managedGameObject->gameObject->GetComponent<CRigidBody>());
			CCollision::GetCollision(managedGameObject->gameObject)->IsTrigger(true);

			if (managedGameObject->gameObject != nullptr)
			{ // ゲームオブジェクトが正常に生成されたとき
				m_managedGameObjects.push_back(managedGameObject);
			}
			else
			{ // ゲームオブジェクトの生成に失敗したとき
				if (managedGameObject != nullptr)
				{
					delete managedGameObject;
					managedGameObject = nullptr;
				}
			}

			// 設置情報にゲームオブジェクト情報を設定する
			(*itrManagedProduces)->managedGameObject = managedGameObject;
		}
		else
		{ // 生成範囲外のとき
			if ((*itrManagedProduces)->managedGameObject != nullptr)
			{
				// ゲームオブジェクトを非アクティブにする
				(*itrManagedProduces)->managedGameObject->gameObject->SetActive(false);

				// 設置オブジェクトから除外する
				(*itrManagedProduces)->managedGameObject = nullptr;
			}
		}
	}


	// 管理ゲームオブジェクトの整理
	for (auto itr = m_managedGameObjects.begin(); itr != m_managedGameObjects.end(); itr++)
	{
		(*itr)->destroyCounter--;
		if ((*itr)->destroyCounter <= 0)
		{ // 破棄カウンターが0になったとき
			ManagedGameObject* pManaged = *itr;
			pManaged->gameObject->Destroy();
			itr = m_managedGameObjects.erase(itr);
			delete pManaged;

			if (itr == m_managedGameObjects.end())
				break;
		}
	}
}

//=============================================================
// [ProducesManager] 近くの生成物を取得する
//=============================================================
float ProducesManager::GetNearProducesRate(const std::string& name, const D3DXVECTOR3& pos, const float& range)
{
	float rate = 1.0f;
	for (auto itr = m_managedProduces.begin(); itr != m_managedProduces.end(); itr++)
	{
		if (Benlib::PosDistance((*itr)->transform.GetPos(), pos) < range)
		{ // 範囲内の時
			rate *= (*itr)->natureProduce->GetAdjacentObjectRate(name);
		}
	}

	return rate;
}

//=============================================================
// [ProducesManager] 指定距離内に生成物があるか
//=============================================================
bool ProducesManager::FindProducesByDistance(const D3DXVECTOR3& pos, const float& distance)
{
	for (auto itr = m_managedProduces.begin(); itr != m_managedProduces.end(); itr++)
	{
		if (Benlib::PosDistance((*itr)->transform.GetPos(), pos) < distance)
		{ // 範囲内の時
			return true;
		}
	}
	return false;
}