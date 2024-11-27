//=============================================================
//
// 地形 [terrain.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "terrain.h"
#include "benlib.h"
#include <fstream>

#include "component/3d/meshfield.h"
#include "scripts/vehicle.h"
#include "scripts/wreckage.h"
#include "component/3d/collision.h"
using namespace noise;

#include "component/2d/text.h"
#include "renderer.h"

// 静的メンバ変数の初期化
const float Terrain::TERRAIN_SCALE = 300.0f;

//=============================================================
// [Terrain] 初期化
//=============================================================
void Terrain::Init()
{
	// メッシュフィールドを作成する
	m_pField = new GameObject;
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);

	m_terrainData = nullptr;
	m_producesManager = new ProducesManager();
	m_producesManager->Init();

	// 障害物を登録する

	// 木
	CProdTree* prodTree = new CProdTree();
	prodTree->SetChance(10);
	prodTree->SetAdjacentRate("tree", 10.0f);
	RegisterProduces(prodTree);
	
	// フェンス
	CProdFence* prodFence = new CProdFence();
	prodFence->SetChance(1);
	prodFence->SetAdjacentRate("fence", 100.0f);
	RegisterProduces(prodFence);

	// 高度カラーを設定する
	AddHeightColor(120.0f, D3DCOLOR_RGBA(11, 112, 0, 255));
	AddHeightColor(-1000.0f, D3DCOLOR_RGBA(50, 166, 8, 255));

	// 傾斜カラーを設定する
	AddSlantColor(300.0f, 500.0f, D3DXCOLOR(0.7f, 0.7f, 0.7f, 1.0f), 0.6f);
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

	// シード値を設定する
	srand(m_seed);

	// 地形データを生成する
	GenerateTerrain();

	// HeightfieldTerrainShapeを作成する
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);

	// 設定したシェープを適用する
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// 生成物を生成する
	for (int i = 0; i < 10000; i++)
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

	myModule.SetSeed(rand());

	utils::NoiseMapBuilderPlane heightMapBuilder;
	heightMapBuilder.SetSourceModule(myModule);
	heightMapBuilder.SetDestNoiseMap(heightMap);
	heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
	heightMapBuilder.SetBounds(2.0, 6.0, 1.0, 5.0);
	heightMapBuilder.Build();

	// 地形情報を格納する
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y) * 1500.0f;
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

			// 色を設定する
			m_pField->GetComponent<CMeshField>()->SetColor(x, y, GetVertexColor(x, y));

			if (m_routeData[x][y])
			{
				m_pField->GetComponent<CMeshField>()->SetColor(x, y, D3DCOLOR_RGBA(179, 119, 0, 255));
			}
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
	// 現在位置情報変数
	int currentX = TERRAIN_SIZE / 2;
	int currentY = TERRAIN_SIZE / 2;
	float currentHeight = GetVertexHeight(currentX, currentY);

	// ルート配列の初期化
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_routeData[x][y] = false;
		}
	}
	m_routeData[currentX][currentY] = true;

	// 経路計算に使う構造体
	struct RouteData
	{
		int x;				// X
		int y;				// Y
		float height;	// 高度
		int point;		// ポイント（低いと優先）
	};

	// 経路アルゴリズム
	while (1)
	{
		//--------------------------------------------------------------------
		// 経路条件

		// 周辺の高さ情報を取得する
		std::vector<RouteData> aroundRouteData;
		for (int x = 0; x < 3; x++)
		{
			for (int y = 0; y < 3; y++)
			{
				if (!(x == 1 && y == 1))
				{ // 中心以外
					RouteData data;
					data.x = currentX - 1 + x;
					data.y = currentY - 1 + y;
					data.height = GetVertexHeight(currentX - 1 + x, currentY - 1 + y);
					aroundRouteData.push_back(data);
				}
			}
		}

		// 高低差でポイントを加算する
		std::sort(aroundRouteData.begin(), aroundRouteData.end(),
			[currentHeight](RouteData& com1, RouteData& com2) {return fabsf(com1.height - currentHeight) < fabsf(com2.height - currentHeight); }
		);
		for (unsigned int i = 0; i < aroundRouteData.size(); i++)
		{
			// 高低差順にポイントを加算する
			aroundRouteData[i].point += i;

			if (aroundRouteData[i].height < currentHeight)
			{ // 現在高度より低いときポイントを引く
				aroundRouteData[i].point -= 3;
			}
		}

		// さらに周辺に道が存在するときポイントを加算する
		for (unsigned int i = 0; i < aroundRouteData.size(); i++)
		{
			for (int x = -1; x < 2; x++)
			{
				for (int y = -1; y < 2; y++)
				{
					// 除外リスト
					if ((x == 1 && y == 1) ||
						(x == currentX && y == currentY))
					{ // 中央と現在位置
						continue;	// スキップ
					}

					if (aroundRouteData[i].x + x >= 0 && aroundRouteData[i].y + y > 0)
					{
						if (m_routeData[aroundRouteData[i].x + x][aroundRouteData[i].y + y])
						{ // 周辺に存在するとき
							aroundRouteData[i].point += 5;
						}
					}
				}
			}
		}


		// 周辺データをポイントを昇順でソートする
		std::sort(aroundRouteData.begin(), aroundRouteData.end(),
			[](RouteData& com1, RouteData& com2) {return com1.point < com2.point;}
		);

		// 最も緩やかな方向に道を伸ばす
		for (unsigned int i = 0; i < aroundRouteData.size(); i++)
		{
			if (!m_routeData[aroundRouteData[i].x][aroundRouteData[i].y])
			{ // 経路になっていないとき
				m_routeData[aroundRouteData[i].x][aroundRouteData[i].y] = true;
				currentX = aroundRouteData[i].x;
				currentY = aroundRouteData[i].y;

				// ファイルに経路データを書き出す
				std::ofstream outputRoute("route.txt");
				for (int y = 0; y < TERRAIN_SIZE; y++)
				{
					for (int x = 0; x < TERRAIN_SIZE; x++)
					{
						char mark = m_routeData[x][y] ? '#' : 'X';
						outputRoute << mark;
					}
					outputRoute << std::endl;
				}
				outputRoute.close();
				break;
			}
		}

		//--------------------------------------------------------------------
		// 終了条件

		// 山の周りにたどり着いたとき
		if (currentX == 0 || currentX == TERRAIN_SIZE - 1 ||
			currentY == 0 || currentY == TERRAIN_SIZE - 1)
		{
			break;	// 終了
		}
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
				m_producesManager->AddProduce(Transform(generatePos), pSelectProduce);

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

	// 高い 色を取得
	D3DXCOLOR highColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	float highHeight = 0.0f;
	bool foundHighHeight = false;
	for (unsigned int i = 0; i < m_heightColor.size(); i++)
	{
		if (height < m_heightColor[i].height &&
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
		if (height > m_heightColor[i].height &&
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
	float currentPercent = (height - lowHeight) / (highHeight - lowHeight);

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
	m_pNumObj = new GameObject("TerrainNumObj");
	m_pNumObj->AddComponent<CText>();
	m_pNumObj->GetComponent<CText>()->SetFontSize(80);
	m_pNumObj->GetComponent<CText>()->SetAlign(CText::ALIGN::RIGHT);
	m_pNumObj->transform->SetPos(CRenderer::SCREEN_WIDTH - 50.0f, 30.0f);
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
	// デバッグ用
	m_pNumObj->GetComponent<CText>()->SetText("生成物総数: " + std::to_string(m_managedGameObjects.size()));

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
		if (Benlib::PosDistance(pos, (*itrManagedProduces)->transform.GetWPos()) < 3500.0f)
		{ // 生成範囲内のとき

			if ((*itrManagedProduces)->managedGameObject != nullptr)
			{ // すでに設置済みのとき
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
						SAFE_ERASE(m_managedProduces, itrManagedProduces);

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
}

//=============================================================
// [ProducesManager] 近くの生成物を取得する
//=============================================================
float ProducesManager::GetNearProduces(const std::string& name, const D3DXVECTOR3& pos, const float& range)
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