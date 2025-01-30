//=============================================================
//
// 装飾 [decoration.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "decoration.h"
#include "scripts/destructible.h"
#include "component/2d/text.h"

#include "scripts/mine.h"
#include "scripts/ring.h"

const float DecorationManager::CHUNK_DIVISION = (Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE) / (float)MAX_CHUNK;
const int DecorationManager::DESTROY_LIMIT = 60;

//=============================================================
// [DecorationManager] 初期化
//=============================================================
void DecorationManager::Init(Terrain* terrain)
{
	m_terrain = terrain;

	m_oldChunkX = -1;
	m_oldChunkY = -1;
}

//=============================================================
// [DecorationManager] 終了
//=============================================================
void DecorationManager::Uninit()
{
	// 種類を解放する
	for (auto itr = m_decoType.begin(); itr != m_decoType.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
			*itr = nullptr;
		}
	}
	m_decoType.clear();

	// 設置データを破棄する
	for (int x = 0; x < MAX_CHUNK; x++)
	{
		for (int y = 0; y < MAX_CHUNK; y++)
		{
			for (auto itr = m_decoData[x][y].begin(); itr != m_decoData[x][y].end(); itr++)
			{
				if (*itr != nullptr)
				{
					delete* itr;
					*itr = nullptr;
				}
			}
			m_decoData[x][y].clear();
		}
	}

	// オブジェクトデータを破棄する
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		if (*itr != nullptr)
		{
			(*itr)->gameObject->Destroy();
			delete* itr;
		}
	}
	m_decoObjects.clear();
}

//=============================================================
// [DecorationManager] 更新
//=============================================================
void DecorationManager::Update(const D3DXVECTOR3& pos)
{
	// 破棄カウンターの更新
	UpdateDestroyObjects();

	// 現在位置のチャンクを取得する
	int x, y;
	GetChunk(&x, &y, pos);

	// 前回から位置が変わっていないとき
	if (m_oldChunkX == x && m_oldChunkY == y)
		return;

	// チャンク
	struct ChunkPos
	{
		int x;
		int y;
	};
	std::vector<ChunkPos> loadChunk;

	// 周辺チャンク 3x3 を読み込み対象にする
	for (int sx = -LOAD_RANGE; sx < LOAD_RANGE + 1; sx++)
	{
		for (int sy = -LOAD_RANGE; sy < LOAD_RANGE + 1; sy++)
		{
			// 範囲チェック
			if (x + sx < 0 || y + sy < 0 ||
				x + sx >= MAX_CHUNK || y + sy >= MAX_CHUNK)
			{ // 範囲外
				continue;
			}
			ChunkPos chunkPos;
			chunkPos.x = x + sx;
			chunkPos.y = y + sy;
			loadChunk.push_back(chunkPos);
		}
	}

	// 範囲から外れたチャンクをアンロードする
	for (int sx = -LOAD_RANGE; sx < LOAD_RANGE + 1; sx++)
	{
		for (int sy = -LOAD_RANGE; sy < LOAD_RANGE + 1; sy++)
		{
			// アンロード対象か
			bool isUnloadTarget = true;
			for (auto itr = loadChunk.begin(); itr != loadChunk.end(); itr++)
			{
				if (sx + m_oldChunkX == (*itr).x &&
					sy + m_oldChunkY == (*itr).y)
				{
					isUnloadTarget = false;
					break;
				}
			}

			// アンロード対象のとき
			if (isUnloadTarget)
			{
				// --- アンロード処理 ---
				UnloadChunk(sx + m_oldChunkX, sy + m_oldChunkY);
			}
		}
	}

	// 新しく読み込むチャンクをロードする
	for (int sx = -LOAD_RANGE; sx < LOAD_RANGE + 1; sx++)
	{
		for (int sy = -LOAD_RANGE; sy < LOAD_RANGE + 1; sy++)
		{
			// 新しいロード対象か
			bool isLoadTarget = true;
			for (int ox = -LOAD_RANGE; ox < LOAD_RANGE + 1; ox++)
			{
				for (int oy = -LOAD_RANGE; oy < LOAD_RANGE + 1; oy++)
				{
					if (sx + x == ox + m_oldChunkX &&
						sy + y == oy + m_oldChunkY)
					{ // すでに読み込まれているとき
						isLoadTarget = false;
						break;
					}
				}
				if (!isLoadTarget) break;
			}

			// ロード対象のとき
			if (isLoadTarget)
			{
				// --- ロード処理 ---
				LoadChunk(sx + x, sy + y);
			}
		}
	}

	// 前回のチャンクとして記録する
	m_oldChunkX = x;
	m_oldChunkY = y;
}

//=============================================================
// [DecorationManager] デコレーションの種類を追加する
//=============================================================
DecorationManager::DecorationType* DecorationManager::AddDecorationType(const std::string& path, const int& chance, const float& offsetY)
{
	DecorationType* type = new DecorationType();
	type->path = path;
	type->chance = chance;
	type->offsetY = offsetY;
	type->slantLimit = { 0.0f, D3DX_PI };
	type->heightLimit = { -1.0f, 1.0f };
	type->randomAngle = 0.0f;
	type->isMatchInclination = true;
	type->radiusSize = 10.0f;
	type->isDestructible = true;
	type->damage = 0.0f;
	type->hitSound = nullptr;
	type->volume = 1.0f;

	m_decoType.push_back(type);
	return type;
}

//=============================================================
// [DecorationManager] データの削除
//=============================================================
void DecorationManager::RemoveData(DecorationData* data)
{
	// チャンクを取得する
	int x, y;
	GetChunk(&x, &y, data->transform.GetPos());

	for (auto itr = m_decoData[x][y].begin(); itr != m_decoData[x][y].end(); itr++)
	{
		if (*itr == data)
		{
			m_decoData[x][y].erase(itr);
			delete data;
			return;
		}
	}

	assert("not found data");
}

//=============================================================
// [DecorationManager] データの削除
//=============================================================
void DecorationManager::RemoveData(GameObject* gameObject)
{
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		if ((*itr)->gameObject == gameObject)
		{
			RemoveData((*itr)->decoDeta);
			break;
		}
	}
}

//=============================================================
// [DecorationManager] デコレーションを生成する
//=============================================================
void DecorationManager::GenerateDecoration()
{
	// デコレーションを決める
	DecorationType* decoType = RandomDecoType();
	if (decoType == nullptr) return;

	// 位置を決める
	D3DXVECTOR3 pos;
	pos.x = Benlib::RandomFloat(-Terrain::TERRAIN_DISTANCE_HALF, Terrain::TERRAIN_DISTANCE_HALF);
	pos.z = Benlib::RandomFloat(-Terrain::TERRAIN_DISTANCE_HALF, Terrain::TERRAIN_DISTANCE_HALF);
	pos.y = 0.0f;

	// チャンクから周囲にオブジェクトがないかを確認する
	int cX, cY;
	GetChunk(&cX, &cY, pos);
	for (auto itr = m_decoData[cX][cY].begin(); itr != m_decoData[cX][cY].end(); itr++)
	{
		if (Benlib::PosDistance((*itr)->transform.GetWPos(), pos) < decoType->radiusSize)
		{
			return;
		}
	}

	// レイポイントの位置を決める
	D3DXVECTOR3 rayPoint[4];
	rayPoint[0].x = -2.0f;
	rayPoint[0].z = 2.0f;
	rayPoint[1].x = 2.0f;
	rayPoint[1].z = 2.0f;
	rayPoint[2].x = 0.0f;
	rayPoint[2].z = -2.0f;
	rayPoint[3].x = 0.0f;
	rayPoint[3].z = 0.0f;

	// 生成座標に移動する
	for (int i = 0; i < 4; i++)
	{
		rayPoint[i] += pos;
	}

	// レイを飛ばす
	D3DXVECTOR3 rayReachPoint[4];
	bool bReached = true;
	for (int i = 0; i < 4; i++)
	{
		btVector3 Start = btVector3(rayPoint[i].x, m_terrain->GetMaxHeight() + 10.0f, rayPoint[i].z);
		btVector3 End = btVector3(rayPoint[i].x, m_terrain->GetMinHeight() - 10.0f, rayPoint[i].z);

		btCollisionWorld::ClosestRayResultCallback RayCallback(Start, End);
		CPhysics::GetInstance()->GetDynamicsWorld().rayTest(Start, End, RayCallback);
		if (RayCallback.hasHit())
		{ // ヒットしたとき
			rayReachPoint[i] = { RayCallback.m_hitPointWorld.getX(), RayCallback.m_hitPointWorld.getY(), RayCallback.m_hitPointWorld.getZ() };
		}
		else
		{
			bReached = false;
			break;
		}
	}

	// レイが当たっていないとき
	if (!bReached)
	{
		return;
	}

	// 位置を更新する
	pos = rayReachPoint[3];

	// 高度制限の条件を満たしていないとき
	float heightRate = (pos.y - m_terrain->GetMinHeight()) / static_cast<float>(m_terrain->GetMaxHeight() - m_terrain->GetMinHeight()) * 2.0f - 1.0f;
	if (!(decoType->heightLimit.min <= heightRate &&
		heightRate <= decoType->heightLimit.max))
	{
		return;
	}

	// 法線ベクトルを計算する
	D3DXVECTOR3 normal = Benlib::CalcNormalVector(rayReachPoint[0], rayReachPoint[1], rayReachPoint[2]);

	// 傾斜制限の条件に満たしていないとき
	if (!(fabsf(atan2f(normal.z, normal.y)) >= decoType->slantLimit.min &&
		fabsf(atan2f(normal.z, normal.y)) <= decoType->slantLimit.max))
	{
		return;
	}

	// 回転を決める ------------------------------------------------------------------------------------------------------------
	D3DXQUATERNION rot;
	D3DXQuaternionIdentity(&rot);

	// ランダム角度を取得する
	float angleRange = decoType->randomAngle;
	std::function<float()> randAngle = [angleRange]() {
		if (angleRange == 0.0f)
			return 0.0f;
		return (rand() % static_cast<int>(angleRange * 1000) - static_cast<int>(angleRange * 500)) * 0.001f;
	};

	// ランダムY軸回転
	float randomY = 0.0f;
	randomY = rand() % static_cast<int>(D3DX_PI * 2 * 10000) * 0.0001f;

	if (!decoType->isMatchInclination)
	{ // 傾斜角を考慮しない
		D3DXVECTOR3 axis = { 0.0f, 1.0f, 0.0f };
		D3DXQuaternionRotationAxis(&rot, &axis, randomY);

		D3DXQUATERNION mulRot;
		D3DXQuaternionRotationYawPitchRoll(&mulRot, randAngle(), 0.0f, randAngle());
		rot *= mulRot;
	}
	else
	{
		D3DXVECTOR3 axis = { 0.0f, 1.0f, 0.0f };
		D3DXQuaternionRotationAxis(&rot, &axis, randomY);

		// 地形に合わせた角度を計算する
		D3DXQUATERNION mulRot;
		axis = { 1.0f, 0.0f, 0.0f };
		D3DXQuaternionRotationAxis(&mulRot, &axis, fabsf(atan2f(normal.z, normal.y)) + randAngle());
		rot *= mulRot;

		D3DXQuaternionIdentity(&mulRot);
		axis = { 0.0f, 1.0f, 0.0f };
		D3DXQuaternionRotationAxis(&mulRot, &axis, atan2f(normal.x, normal.z) + randAngle());
		rot *= mulRot;
	}

	// トランスフォームに設定する
	Transform transform;
	transform.SetPos(pos+D3DXVECTOR3(0.0f, decoType->offsetY, 0.0f));
	transform.SetQuaternion(rot);

	// 設置する（データ追加）
	AddDecorationData(decoType, transform);
}

//=============================================================
// [DecorationManager] デコレーションデータを追加する
//=============================================================
void DecorationManager::AddDecorationData(DecorationType* decoType, Transform transform)
{
	DecorationData* data = new DecorationData();
	data->type = decoType;
	data->transform = transform;
	
	// 位置からチャンクを決める
	int x, y;
	GetChunk(&x, &y, transform.GetPos());

	// データを追加する
	m_decoData[x][y].push_back(data);
}

//=============================================================
// [DecorationManager] ランダムでデコレーションの種類を決める
//=============================================================
DecorationManager::DecorationType* DecorationManager::RandomDecoType()
{
	int nAllChance = 0;
	for (auto itr = m_decoType.begin(); itr != m_decoType.end(); itr++)
	{
		nAllChance += (*itr)->chance;
	}

	int nRandValue = rand() % nAllChance;	// ランダム値
	int nMinChance = 0;								// チャンス範囲の最小値
	int nMaxChance = 0;								// チャンス範囲の最大値
	for (auto itr = m_decoType.begin(); itr != m_decoType.end(); itr++)
	{
		nMaxChance = nMinChance + (*itr)->chance;
		if (nMinChance <= nRandValue && nRandValue < nMaxChance)
		{ // 範囲内のとき
			return *itr;
		}
		nMinChance += (*itr)->chance;
	}

	return nullptr;
}

//=============================================================
// [DecorationManager] 位置からチャンクを取得する
//=============================================================
void DecorationManager::GetChunk(int* x, int* y, const D3DXVECTOR3& pos)
{
	// 位置からチャンクを決める
	int sx, sy;
	sx = static_cast<int>((pos.x + Terrain::TERRAIN_DISTANCE_HALF) / (float)CHUNK_DIVISION);
	sy = static_cast<int>((pos.z + Terrain::TERRAIN_DISTANCE_HALF) / (float)CHUNK_DIVISION);
	if (sx >= MAX_CHUNK) sx = MAX_CHUNK - 1;
	if (sy >= MAX_CHUNK) sy = MAX_CHUNK - 1;
	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;

	// 返す
	*x = sx;
	*y = sy;
}

//=============================================================
// [DecorationManager] チャンクをロードする
//=============================================================
void DecorationManager::LoadChunk(const int& x, const int& y)
{
	if (0 <= x && x < MAX_CHUNK &&
		0 <= y && y < MAX_CHUNK)
	{
		for (auto itr = m_decoData[x][y].begin(); itr != m_decoData[x][y].end(); itr++)
		{
			ActiveData(*itr);
		}
	}
}

//=============================================================
// [DecorationManager] チャンクをアンロードする
//=============================================================
void DecorationManager::UnloadChunk(const int& x, const int& y)
{
	if (0 <= x && x < MAX_CHUNK &&
		0 <= y && y < MAX_CHUNK)
	{
		for (auto itr = m_decoData[x][y].begin(); itr != m_decoData[x][y].end(); itr++)
		{
			PassiveData(*itr);
		}
	}
}

//=============================================================
// [DecorationManager] データを有効化する
//=============================================================
void DecorationManager::ActiveData(DecorationData* decoData)
{
	DecorationObject* targetDecoObj = nullptr;
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		// すでに読み込まれているとき
		if ((*itr)->decoDeta == decoData)
		{
			return;
		}

		// 空のオブジェクトを見つけたとき（種類一致）
		if (targetDecoObj == nullptr && 
			(*itr)->decoDeta == nullptr && (*itr)->decoType == decoData->type &&
			(*itr)->gameObject != nullptr && (*itr)->gameObject->transform != nullptr)
		{
			targetDecoObj = *itr;
		}
	}
	
	if (targetDecoObj == nullptr)
	{ // 条件に合うオブジェクトがなかったとき（作成）
		targetDecoObj = new DecorationObject();
		targetDecoObj->decoDeta = decoData;
		targetDecoObj->decoType = decoData->type;
		targetDecoObj->gameObject = GameObject::LoadPrefab(decoData->type->path, decoData->transform);
		targetDecoObj->destroyCounter = DESTROY_LIMIT;

		// 破壊可能オブジェクトのとき
		if (decoData->type->isDestructible)
		{
			targetDecoObj->gameObject->AddComponent<Destructible>(this)->SetDecoData(decoData);
			targetDecoObj->gameObject->AddComponent<AudioSource>();
		}

		// 特殊タグの場合
		if (targetDecoObj->gameObject->GetTag() == "MINE")
		{ // 地雷
			targetDecoObj->gameObject->AddComponent<LandMine>();
		}
		else if (targetDecoObj->gameObject->GetTag() == "RING")
		{ // リング
			targetDecoObj->gameObject->AddComponent<PointRing>();
		}

		m_decoObjects.push_back(targetDecoObj);
	}
	else
	{ // 条件に合うオブジェクトが見つかったとき
		if (!GameObject::IsExist(targetDecoObj->gameObject)) return;

		targetDecoObj->decoDeta = decoData;
		targetDecoObj->gameObject->transform->SetPos(decoData->transform.GetPos());
		targetDecoObj->gameObject->transform->SetQuaternion(decoData->transform.GetQuaternion());
		targetDecoObj->gameObject->transform->SetScale(decoData->transform.GetScale());
		targetDecoObj->destroyCounter = DESTROY_LIMIT;
		if (targetDecoObj->gameObject->GetComponent<Destructible>() != nullptr)
		{
			targetDecoObj->gameObject->GetComponent<Destructible>()->SetDecoData(decoData);
		}
	}

	// ゲームオブジェクトをアクティブにする
	targetDecoObj->gameObject->SetActive(true);
}

//=============================================================
// [DecorationManager] データを無効化する
//=============================================================
void DecorationManager::PassiveData(DecorationData* decoData)
{
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		if ((*itr)->decoDeta == decoData)
		{
			(*itr)->decoDeta = nullptr;
			(*itr)->gameObject->SetActive(false);
			(*itr)->destroyCounter = DESTROY_LIMIT;
			return;
		}
	}
}

//=============================================================
// [DecorationManager] 使われていないオブジェクトの破棄カウンター更新処理
//=============================================================
void DecorationManager::UpdateDestroyObjects()
{
	for (auto itr = m_decoObjects.begin(); itr != m_decoObjects.end(); itr++)
	{
		DecorationObject* decoObj = *itr;
		if (decoObj->decoDeta == nullptr)
		{
			decoObj->destroyCounter--;

			// 破棄タイミングが来たとき
			if (decoObj->destroyCounter <= 0)
			{ 
				decoObj->gameObject->Destroy();
				delete decoObj;
				itr = m_decoObjects.erase(itr);
				if (itr == m_decoObjects.end()) return;
			}
		}
	}
}

//=============================================================
// [DecorationManager] 地形ファイルを読み込む
//=============================================================
#include "manager.h"
#include <fstream>
void DecorationManager::LoadTerrainFile(const std::string path)
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

	// 生成数を読み込む
	m_generateNum = 0;
	if (jInput["terrain"].contains("generate_num"))
	{
		m_generateNum = jInput["terrain"]["generate_num"];
	}
	

	// 生成物情報を読み込む
	if (jInput.contains("decoration"))
	{ // 生成物の項目があるとき
		for (auto itr = jInput["decoration"].begin(); itr != jInput["decoration"].end(); itr++)
		{
			DecorationType* decoType = AddDecorationType((*itr)["path"]);

			// サイズ
			if ((*itr).contains("radius"))
			{
				decoType->radiusSize = (*itr)["radius"];
			}

			// 破壊設定
			if ((*itr).contains("destructible"))
			{
				decoType->isDestructible = (*itr)["destructible"];
			}

			// ダメージ
			if ((*itr).contains("damage"))
			{
				decoType->damage = (*itr)["damage"];
			}

			// オフセットY
			if ((*itr).contains("offset"))
			{
				decoType->offsetY = (*itr)["offset"];
			}

			// ランダム角度
			if ((*itr).contains("angle_range"))
			{
				decoType->randomAngle = (*itr)["angle_range"];
			}

			// 傾斜角に合わせるか
			if ((*itr).contains("match_inclination"))
			{
				decoType->isMatchInclination = (*itr)["match_inclination"];
			}

			// 確率
			if ((*itr).contains("chance"))
			{
				decoType->chance = (*itr)["chance"];
			}

			// 傾斜角度条件
			if ((*itr).contains("limit_slant"))
			{
				decoType->slantLimit.min = (*itr)["limit_slant"][0];
				decoType->slantLimit.max = (*itr)["limit_slant"][1];
			}

			// 高度条件
			if ((*itr).contains("limit_height"))
			{
				decoType->heightLimit.min = (*itr)["limit_height"][0];
				decoType->heightLimit.max = (*itr)["limit_height"][1];
			}

			// 衝突音
			if ((*itr).contains("hit_sound"))
			{
				// ループ
				if ((*itr).contains("loop") && (*itr)["loop"])
				{ // 有効
					decoType->hitSound = AudioManager::GetInstance()->CreateClip((*itr)["hit_sound"], FMOD_3D | FMOD_LOOP_NORMAL);
				}
				else
				{ // 無効
					decoType->hitSound = AudioManager::GetInstance()->CreateClip((*itr)["hit_sound"], FMOD_3D);
				}
				
				
				// 音量
				if ((*itr).contains("volume"))
				{
					decoType->volume = (*itr)["volume"];
				}
			}
		}
	}

	ifs.close();
}

//=============================================================
// [DecorationManager] 生成する
//=============================================================
void DecorationManager::Generate()
{
	for (int i = 0; i < m_generateNum; i++)
	{
		GenerateDecoration();
	}
}