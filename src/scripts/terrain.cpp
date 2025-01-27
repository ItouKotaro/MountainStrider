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
const float Terrain::TERRAIN_DISTANCE = Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE;
const float Terrain::TERRAIN_DISTANCE_HALF = Terrain::TERRAIN_DISTANCE / (float)2.0f;

//=============================================================
// [Terrain] 初期化
//=============================================================
void Terrain::Init()
{
	// メッシュフィールドを作成する
	m_pField = new GameObject("TerrainField", "Field");
	m_pField->AddComponent<CMeshField>()->Create(TERRAIN_SIZE - 1, TERRAIN_SIZE - 1, TERRAIN_SCALE);
	m_pField->AddComponent<Road>();

	m_pLimitField = new GameObject("LimitField");
	m_pLimitField->AddComponent<CBoxCollider>(D3DXVECTOR3(TERRAIN_SCALE * (TERRAIN_SIZE - 3), 1.0f, TERRAIN_SCALE * (TERRAIN_SIZE - 3)));

	// 当たり判定を更新する
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	m_terrainData = nullptr;
	m_lakeEnabled = false;
	m_lakeHeight = 0.0f;
}

//=============================================================
// [Terrain] 終了
//=============================================================
void Terrain::Uninit()
{
	// 地形の破棄
	UninitTerrain();
}

//=============================================================
// [Terrain] 更新
//=============================================================
void Terrain::Update()
{
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

	CDataManager::GetInstance()->RemoveData("data\\terrain.bmp");
	m_pField->GetComponent<CMeshField>()->SetTexture("data\\terrain.bmp");

	// HeightfieldTerrainShapeを作成する
	m_terrainShape = new btHeightfieldTerrainShape(TERRAIN_SIZE, TERRAIN_SIZE, m_terrainData, 1, -30000, 30000, 1, PHY_FLOAT, false);
	m_terrainShape->setLocalScaling(btVector3(TERRAIN_SCALE, 1.0f, TERRAIN_SCALE));
	CCollision::GetCollision(m_pField)->SetFriction(100.0f);
	CCollision::GetCollision(m_pField)->GetGhostObject()->setCollisionShape(m_terrainShape);

	// 設定したシェープを適用する
	CPhysics::GetInstance()->GetDynamicsWorld().stepSimulation(static_cast<btScalar>(1. / 60.), 1);

	// 道のオブジェクトを生成する
	m_pField->GetComponent<Road>()->Generate();

	// ジェムの生成
	for (int i = 0; i < 150; i++)
	{
		GenerateGem();
	}
}

//=============================================================
// [Terrain] 地形の生成
//=============================================================
void Terrain::GenerateTerrain()
{
	utils::NoiseMap heightMap;
	utils::NoiseMapBuilderPlane heightMapBuilder;

	// 地形IDに基づく生成処理
	if (m_terrainID == 0)
	{
		module::Perlin myModule;
		myModule.SetSeed(rand());
		module::ScaleBias scaled;
		scaled.SetSourceModule(0, myModule);
		scaled.SetScale(100.0f);

		// 地形を生成する
		heightMapBuilder.SetSourceModule(scaled);
		heightMapBuilder.SetDestNoiseMap(heightMap);
		heightMapBuilder.SetDestSize(TERRAIN_SIZE, TERRAIN_SIZE);
		heightMapBuilder.SetBounds(0.0, 20.0, 0.0, 20.0);
		heightMapBuilder.Build();
	}

	// テクスチャ画像に書き出す
	utils::RendererImage renderer;
	utils::Image image;
	renderer.SetSourceNoiseMap(heightMap);
	renderer.SetDestImage(image);
	renderer.ClearGradient();
	for (auto itr = m_heightColor.begin(); itr != m_heightColor.end(); itr++)
	{
		renderer.AddGradientPoint((*itr).height, utils::Color(static_cast<noise::uint8>((*itr).color.r * 255), static_cast<noise::uint8>((*itr).color.g * 255), static_cast<noise::uint8>((*itr).color.b * 255), 255));
	}
	renderer.EnableLight();
	renderer.SetLightContrast(0.01f);
	renderer.SetLightBrightness(1.5);
	renderer.SetLightIntensity(4.5f);
	renderer.Render();

	// 結果表示用画像
	utils::RendererImage convenienceRenderer;
	utils::Image convenienceImage;
	convenienceRenderer.SetSourceNoiseMap(heightMap);
	convenienceRenderer.SetDestImage(convenienceImage);
	convenienceRenderer.ClearGradient();
	for (auto itr = m_heightColor.begin(); itr != m_heightColor.end(); itr++)
	{
		convenienceRenderer.AddGradientPoint((*itr).height, utils::Color(static_cast<noise::uint8>((*itr).color.r * 255), static_cast<noise::uint8>((*itr).color.g * 255), static_cast<noise::uint8>((*itr).color.b * 255), 255));
	}
	convenienceRenderer.Render();

	// ファイルに書き出す
	utils::WriterBMP writer;
	writer.SetSourceImage(image);
	writer.SetDestFilename("data\\terrain.bmp");
	writer.WriteDestFile();

	writer.SetSourceImage(convenienceImage);
	writer.SetDestFilename("data\\convenience.bmp");
	writer.WriteDestFile();


	// 地形情報を格納する
	m_terrainData = new float[TERRAIN_SIZE * TERRAIN_SIZE];
	for (int x = 0; x < TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < TERRAIN_SIZE; y++)
		{
			m_terrainData[x + (TERRAIN_SIZE - 1 - y) * TERRAIN_SIZE] = heightMap.GetValue(x, y);
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

	// 制限の高さを変更する
	m_pLimitField->transform->SetPos(0.0f, m_minHeight - 5.0f, 0.0f);
	CCollision::GetCollision(m_pLimitField)->GetGhostObject()->getWorldTransform().getOrigin() = btVector3(0.0f, m_minHeight - 5.0f, 0.0f);

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
			//m_pShadowField->GetComponent<CMeshField>()->SetHeight(x, y, height);
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

	// 湖はポイントを減点する
	if (m_lakeEnabled)
	{
		float rate = (m_lakeHeight + 1.0f) / static_cast<float>(2.0f);
		float lakeHeight = m_minHeight + (m_maxHeight - m_minHeight) * rate;

		for (int x = 0; x < TERRAIN_SIZE; x++)
		{
			for (int y = 0; y < TERRAIN_SIZE; y++)
			{
				float height = GetVertexHeight(x, y);
				if (height <= lakeHeight)
				{
					// 減点
					routeData[x][y] -= 400;
				}
			}
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
						routeData[currentX + x][currentY + y] = static_cast<int>(routeData[currentX + x][currentY + y] * 0.2f);
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
		// 地形ID
		m_terrainID = 0;
		if (jInput["terrain"].contains("id"))
		{
			m_terrainID = jInput["terrain"]["id"];
		}

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

		// 湖の高度を取得する
		if (jInput["terrain"].contains("lake"))
		{
			m_lakeEnabled = true;

			if (jInput["terrain"]["lake"].contains("height"))
			{
				m_lakeHeight = jInput["terrain"]["lake"]["height"];
			}
		}
	}

	ifs.close();
}