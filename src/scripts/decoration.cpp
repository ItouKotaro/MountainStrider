//=============================================================
//
// 装飾 [decoration.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "decoration.h"

#include "component/2d/text.h"

const float DecorationManager::CHUNK_DIVISION = (Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE) / (float)MAX_CHUNK;
const int DecorationManager::DESTROY_LIMIT = 60;
//=============================================================
// [DecorationManager] 初期化
//=============================================================
void DecorationManager::Init(Terrain* terrain)
{
	m_terrain = terrain;

	m_oldChunkX = 0;
	m_oldChunkY = 0;

	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
		{
			LoadChunk(MAX_CHUNK / 2 + sx, MAX_CHUNK / 2 + sy);
		}
	}

	m_debugText = new GameObject();
	m_debugText->AddComponent<CText>();
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
	m_debugText->GetComponent<CText>()->SetText(std::to_string(m_decoObjects.size()));

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
	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
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
	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
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
	for (int sx = -1; sx < 2; sx++)
	{
		for (int sy = -1; sy < 2; sy++)
		{
			// 新しいロード対象か
			bool isLoadTarget = true;
			for (int ox = -1; ox < 2; ox++)
			{
				for (int oy = -1; oy < 2; oy++)
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
void DecorationManager::AddDecorationType(const std::string& path, const int& chance, const float& offsetY)
{
	DecorationType* type = new DecorationType();
	type->path = path;
	type->chance = chance;
	type->offsetY = offsetY;
	type->minSlant = 0.0f;
	type->maxSlant = D3DX_PI;
	m_decoType.push_back(type);
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

	// 法線ベクトルを計算する
	D3DXVECTOR3 normal = Benlib::CalcNormalVector(rayReachPoint[0], rayReachPoint[1], rayReachPoint[2]);

	// 傾斜制限の条件に満たしていないとき
	if (!(fabsf(atan2f(normal.z, normal.y)) >= decoType->minSlant &&
		fabsf(atan2f(normal.z, normal.y)) <= decoType->maxSlant))
	{
		return;
	}

	// 回転を決める ------------------------------------------------------------------------------------------------------------
	D3DXQUATERNION rot;
	D3DXQuaternionIdentity(&rot);

	// ランダム角度を取得する
	float angleRange = 0.0f;
	std::function<float()> randAngle = [angleRange]() {
		if (angleRange == 0)
			return 0.0f;
		return (rand() % static_cast<int>(angleRange * 1000) - static_cast<int>(angleRange * 500)) * 0.001f;
	};

	// 地形に合わせた角度を計算する
	D3DXVECTOR3 axis = { 1.0f, 0.0f, 0.0f };
	D3DXQuaternionRotationAxis(&rot, &axis, fabsf(atan2f(normal.z, normal.y)) + randAngle());

	D3DXQUATERNION mulRot;
	axis = { 0.0f, 1.0f, 0.0f };
	D3DXQuaternionRotationAxis(&mulRot, &axis, atan2f(normal.x, normal.z) + randAngle());
	rot *= mulRot;


	// トランスフォームに設定する
	Transform transform;
	transform.SetPos(pos);
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
			(*itr)->decoDeta == nullptr && (*itr)->decoType == decoData->type)
		{
			targetDecoObj = *itr;
		}
	}
	
	if (targetDecoObj == nullptr)
	{ // 条件に合うオブジェクトがなかった時
		targetDecoObj = new DecorationObject();
		targetDecoObj->decoDeta = decoData;
		targetDecoObj->decoType = decoData->type;
		targetDecoObj->gameObject = GameObject::LoadPrefab(decoData->type->path, decoData->transform);
		targetDecoObj->destroyCounter = DESTROY_LIMIT;
		m_decoObjects.push_back(targetDecoObj);
	}
	else
	{ // 条件に合うオブジェクトが見つかったとき
		targetDecoObj->decoDeta = decoData;
		targetDecoObj->gameObject->transform->SetPos(decoData->transform.GetPos());
		targetDecoObj->gameObject->transform->SetQuaternion(decoData->transform.GetQuaternion());
		targetDecoObj->gameObject->transform->SetScale(decoData->transform.GetScale());
		targetDecoObj->destroyCounter = DESTROY_LIMIT;
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