//=============================================================
//
// 軌道 [trajectory.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "trajectory.h"
#include "manager.h"

const float CTrajectory::FADE = 0.15f;
const char* CTrajectory::TEX_PATH = "data\\TEXTURE\\PARTICLE\\trajectory00.jpg";

//=============================================================
// [CTrajectory] 初期化
//=============================================================
void CTrajectory::Init()
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < MAX_FRAME; i++)
	{
		m_trajectoryData[i].rootPos = { 0.0f, 0.0f, 0.0f };
		m_trajectoryData[i].tipPos = { 0.0f, 0.0f, 0.0f };
	}

	// テクスチャの読み込み
	D3DXCreateTextureFromFile(pDevice, TEX_PATH, &m_pTextureTrajectory);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MAX_FRAME * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffTrajectory, NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * (2 * MAX_FRAME + 1),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffTrajectory,
		nullptr);

	VERTEX_3D* pVtx;
	int nVertexLine = -1;	// 現在の列

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);

	int nCounter = 0;
	float fXTexSize = 1.0f / (float)MAX_FRAME;
	for (int nCntVertex = 0; nCntVertex < MAX_FRAME * 2; nCntVertex++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(200, 200, 200, 180);

		// テクスチャ座標
		if (nCntVertex < MAX_FRAME)
		{
			pVtx[0].tex = D3DXVECTOR2(fXTexSize * (nCntVertex % MAX_FRAME), 0.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(fXTexSize * (nCntVertex % MAX_FRAME), 1.0f);
		}

		pVtx++;
	}

	// アンロック
	m_pVtxBuffTrajectory->Unlock();

	// インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuffTrajectory->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	for (int nCntIdxWidth = 0; nCntIdxWidth < MAX_FRAME; nCntIdxWidth++)
	{
		pIdx[0] = (MAX_FRAME - 1 + nCntIdxWidth) + 1;
		pIdx[1] = nCntIdxWidth;

		pIdx += 2;
	}

	// 折り返し
	pIdx[0] = (MAX_FRAME + 1) - 1;
	pIdx += 1;

	// アンロック
	m_pIdxBuffTrajectory->Unlock();
}

//=============================================================
// [CTrajectory] 終了
//=============================================================
void CTrajectory::Uninit()
{
	// テクスチャの破棄
	if (m_pTextureTrajectory != nullptr)
	{
		m_pTextureTrajectory->Release();
		m_pTextureTrajectory = nullptr;
	}

	// 頂点バッファの破棄
	if (m_pVtxBuffTrajectory != nullptr)
	{
		m_pVtxBuffTrajectory->Release();
		m_pVtxBuffTrajectory = nullptr;
	}

	// インデックスバッファの破棄
	if (m_pIdxBuffTrajectory != nullptr)
	{
		m_pIdxBuffTrajectory->Release();
		m_pIdxBuffTrajectory = nullptr;
	}
}

//=============================================================
// [CTrajectory] 更新
//=============================================================
void CTrajectory::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	// デバイスを取得

	VERTEX_3D* pVtx;
	int nVertexLine = -1;	// 現在の列

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVertex = 0; nCntVertex < MAX_FRAME * 2; nCntVertex++)
	{
		// 頂点座標の設定
		if (nCntVertex < MAX_FRAME)
		{
			pVtx[0].pos = m_trajectoryData[nCntVertex % MAX_FRAME].tipPos;
		}
		else
		{
			pVtx[0].pos = m_trajectoryData[nCntVertex % MAX_FRAME].rootPos;
		}

		D3DXCOLOR col = pVtx[0].col;
		col.a = 1.0f - (nCntVertex % MAX_FRAME) * FADE;
		pVtx[0].col = col;

		pVtx++;
	}

	// アンロック
	m_pVtxBuffTrajectory->Unlock();
}

//=============================================================
// [CTrajectory] 描画
//=============================================================
void CTrajectory::Draw()
{
	if (m_bShow == false)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtxRot, mtxTrans;							// 計算用マトリックス

	Component::BeginPass();

	//αブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorldTrajectory);

	// 位置を反映
	D3DXMatrixTranslation(&m_mtxWorldTrajectory, 0.0f, 0.0f, 0.0f);

	// ワールドマトリックスの設定
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldTrajectory);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuffTrajectory, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuffTrajectory);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_pTextureTrajectory);

	// ポリゴン描画前に設定
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	Shader::ParamData paramData;
	paramData.color = {1.0f, 1.0f, 1.0f, 1.0f};
	paramData.texture = m_pTextureTrajectory;
	paramData.mtx = transform->GetMatrix();
	Component::SetParam(paramData);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0,
		0,
		2 * (MAX_FRAME - 1) + 2,
		0,
		(MAX_FRAME - 1) * 2);

	Component::EndPass();

	// ポリゴン描画後に元に戻す
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================
// [CTrajectory] 軌道の追加
//=============================================================
void CTrajectory::AddTrajectory(const D3DXVECTOR3& root, const D3DXVECTOR3& tip)
{
	m_trajectoryData[MAX_FRAME - 1].rootPos = { 0.0f, 0.0f, 0.0f };
	m_trajectoryData[MAX_FRAME - 1].tipPos = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < MAX_FRAME; i++)
	{
		if (MAX_FRAME - (2 + i) >= 0)
		{
			m_trajectoryData[MAX_FRAME - (1 + i)].rootPos = m_trajectoryData[MAX_FRAME - (2 + i)].rootPos;
			m_trajectoryData[MAX_FRAME - (1 + i)].tipPos = m_trajectoryData[MAX_FRAME - (2 + i)].tipPos;
		}
	}

	m_trajectoryData[0].rootPos = root;
	m_trajectoryData[0].tipPos = tip;
}

//=============================================================
// [CTrajectory] 表示変更
//=============================================================
void CTrajectory::SetShow(const bool& bEnabled)
{
	m_bShow = bEnabled;

	if (bEnabled == false)
	{
		for (int i = 0; i < MAX_FRAME; i++)
		{
			m_trajectoryData[i].tipPos = { 0.0f, 0.0f, 0.0f };
			m_trajectoryData[i].rootPos = { 0.0f, 0.0f, 0.0f };
		}
	}
}

//=============================================================
// [CTrajectory] 軌道位置のリセット
//=============================================================
void CTrajectory::ResetPos(const D3DXVECTOR3& pos)
{
	for (int i = 0; i < MAX_FRAME; i++)
	{
		m_trajectoryData[i].tipPos = pos;
		m_trajectoryData[i].rootPos = pos;
	}
}