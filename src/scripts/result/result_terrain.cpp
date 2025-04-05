//=============================================================
//
// リザルトの地形表示 [result_terrain.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_terrain.h"
#include "renderer.h"
#include "scene/game.h"
#include "scripts/terrain.h"
#include "component/2d/text.h"

const std::string ResultTerrain::TERRAIN_TEX = "data\\convenience.bmp";
const int ResultTerrain::SIZE = 500;
const float ResultTerrain::TRAVELLING_POINT_SIZE = 5.0f;
const int ResultTerrain::TRAVELLING_FRAME = 1;
const int ResultTerrain::TRAVELLING_ENDFRAME = 120;

//=============================================================
// [ResultTerrain] 初期化
//=============================================================
void ResultTerrain::Init()
{
	int terrainSize = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain()->GetTerrainSize();
	float terrainScale = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain()->GetTerrainScale();

	m_terrainVtxBuff = nullptr;
	m_texture = nullptr;
	m_travellingCounter = 0;
	m_pointSize = static_cast<float>(SIZE) / terrainSize;

	// 走行データを取得する
	auto travellingData = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTravellingData();
	if (travellingData.size() <= 0)
	{
		return;
	}


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// テクスチャの生成
	D3DXCreateTextureFromFile(pDevice, TERRAIN_TEX.c_str(), &m_texture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_terrainVtxBuff, nullptr);
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_terrainVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(SIZE * -0.5f, SIZE * -0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SIZE * 0.5f, SIZE * -0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SIZE * -0.5f, SIZE * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SIZE * 0.5f, SIZE * 0.5f, 0.0f);

	// サイズと回転
	D3DXMATRIX rollMtx;
	D3DXMatrixIdentity(&rollMtx);
	D3DXMatrixRotationZ(&rollMtx, MAP_ROLL);
	D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &rollMtx);
	D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &rollMtx);
	D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &rollMtx);
	D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &rollMtx);

	pVtx[0].pos += transform->GetWPos();
	pVtx[1].pos += transform->GetWPos();
	pVtx[2].pos += transform->GetWPos();
	pVtx[3].pos += transform->GetWPos();

	// rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// 頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// 頂点バッファをアンロックする
	m_terrainVtxBuff->Unlock();

	// 走行データに基づいて頂点を生成する
	unsigned int numData = static_cast<unsigned int>(travellingData.size());
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * numData, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_travellingVtxBuff, nullptr);

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_travellingVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (unsigned int i = 0; i < numData; i++)
	{
		// 頂点座標の設定
		D3DXVECTOR3 pos = (travellingData[i].pos / static_cast<float>((terrainSize * terrainScale))) * SIZE;
		pos.y = pos.z;
		pos.z = 0.0f;
		pVtx[0].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * -0.5f, TRAVELLING_POINT_SIZE * -0.5f, 0.0f) + pos;
		pVtx[1].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * 0.5f, TRAVELLING_POINT_SIZE * -0.5f, 0.0f) + pos;
		pVtx[2].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * -0.5f, TRAVELLING_POINT_SIZE * 0.5f, 0.0f) + pos;
		pVtx[3].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * 0.5f, TRAVELLING_POINT_SIZE * 0.5f, 0.0f) + pos;

		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// 次のポインタへ
		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	m_travellingVtxBuff->Unlock();

	// 池マップの初期化
	InitLakeMap();
}



//=============================================================
// [ResultTerrain] 終了
//=============================================================
void ResultTerrain::Uninit()
{
	// テクスチャの破棄
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	// 地形の破棄
	if (m_terrainVtxBuff != nullptr)
	{
		m_terrainVtxBuff->Release();
		m_terrainVtxBuff = nullptr;
	}

	// 走行マップの破棄
	if (m_travellingVtxBuff != nullptr)
	{
		m_travellingVtxBuff->Release();
		m_travellingVtxBuff = nullptr;
	}

	// 池マップの破棄
	if (m_lakeVtxBuff != nullptr)
	{
		m_lakeVtxBuff->Release();
		m_lakeVtxBuff = nullptr;
	}
}

//=============================================================
// [ResultTerrain] 更新
//=============================================================
void ResultTerrain::Update()
{
	auto travellingData = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTravellingData();
	if (travellingData.size() <= 0)
	{
		return;
	}

	m_travellingCounter--;
	if (m_travellingCounter <= 0)
	{ // 走行カウントが達したとき
		m_travellingIdx++;

		if (m_travellingIdx >= travellingData.size() - 1)
		{ // 最終ポイントのとき
			m_travellingCounter = TRAVELLING_ENDFRAME;
		}
		else
		{ // 通常
			m_travellingCounter = TRAVELLING_FRAME;
		}

		if (m_travellingIdx >= travellingData.size())
		{ // インデックスが最大値に達したとき
			m_travellingIdx = 0;
		}
	}
}

//=============================================================
// [ResultTerrain] 描画
//=============================================================
void ResultTerrain::DrawUI()
{
	auto travellingData = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTravellingData();
	if (travellingData.size() <= 0)
	{
		return;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_terrainVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_texture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数


	// 池データの描画
	for (int i = 0; i < m_numLake; i++)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_lakeVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		// ポリゴンの描画
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
			i * 4, //描画する最初の頂点インデックス
			2); //描画するプリミティブ数
	}


	// 走行データの描画
	for (unsigned int i = 0; i < travellingData.size(); i++)
	{
		// 頂点バッファをデータストリームに設定
		pDevice->SetStreamSource(0, m_travellingVtxBuff, 0, sizeof(VERTEX_2D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_2D);

		// テクスチャの設定
		pDevice->SetTexture(0, nullptr);

		// ポリゴンの描画
		if (i <= m_travellingIdx)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
				i * 4, //描画する最初の頂点インデックス
				2); //描画するプリミティブ数
		}
	}
}

//=============================================================
// [ResultTerrain] 池マップの初期化
//=============================================================
void ResultTerrain::InitLakeMap()
{
	struct LakePos
	{
		int x, y;
	};

	// 池の位置を取得する
	auto terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();
	auto lake = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetLake();
	std::vector<LakePos> lakePosList;
	m_numLake = 0;

	// 池がないときは終了する
	if (!lake->IsEnabled())
		return;

	for (int x = 0; x < terrain->GetTerrainSize(); x++)
	{
		for (int y = 0; y < terrain->GetTerrainSize(); y++)
		{
			float height = lake->GetHeight();
			float current = terrain->GetVertexHeight(x, y);
			if (current < height)
			{
				LakePos pos;
				pos.x = (terrain->GetTerrainSize() - 1) -x;
				pos.y = y;
				lakePosList.push_back(pos);
				m_numLake++;
			}
		}
	}

	if (m_numLake <= 0)
		return;


	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * m_numLake, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_lakeVtxBuff, nullptr);
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_lakeVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (auto itr = lakePosList.begin(); itr != lakePosList.end(); itr++)
	{
		D3DXVECTOR3 putPos;
		putPos.x = (*itr).x * m_pointSize - SIZE *0.5f;
		putPos.y = (*itr).y * m_pointSize - SIZE * 0.5f;
		putPos.z = 0.0f;

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + putPos ;
		pVtx[1].pos = D3DXVECTOR3(m_pointSize, 0.0f, 0.0f) + putPos;
		pVtx[2].pos = D3DXVECTOR3(0.0f, m_pointSize, 0.0f) + putPos;
		pVtx[3].pos = D3DXVECTOR3(m_pointSize, m_pointSize, 0.0f) + putPos;

		// サイズと回転
		D3DXMATRIX rollMtx;
		D3DXMatrixIdentity(&rollMtx);
		D3DXMatrixRotationZ(&rollMtx, LAKE_ROLL);
		D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &rollMtx);
		D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &rollMtx);
		D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &rollMtx);
		D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &rollMtx);

		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

		// rhwの設定
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// 頂点カラー
		pVtx[0].col = lake->GetBaseColor();
		pVtx[1].col = lake->GetBaseColor();
		pVtx[2].col = lake->GetBaseColor();
		pVtx[3].col = lake->GetBaseColor();

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// 頂点バッファをアンロックする
	m_lakeVtxBuff->Unlock();
}
