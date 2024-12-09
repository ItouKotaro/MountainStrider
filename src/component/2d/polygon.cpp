//=============================================================
//
// 画像コンポーネント [polygon.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "polygon.h"
#include "renderer.h"
#include "manager.h"

//=============================================================
// [CPolygon] コンストラクタ
//=============================================================
CPolygon::CPolygon()
{
	m_pVtxBuff = nullptr;
	m_pTexture = nullptr;
	m_anchorPoint = { 0.0f, 0.0f };
	m_nSplit = 1;
	m_nTime = 0;
	m_nextTime = timeGetTime();
	m_nAnimIdx = 0;
	m_bReverse = false;
}

//=============================================================
// [CPolygon] 引数付きコンストラクタ
//=============================================================
CPolygon::CPolygon(std::string sTexPath) : CPolygon()
{
	SetTexture(sTexPath.c_str());
}

//=============================================================
// [CPolygon] デストラクタ
//=============================================================
CPolygon::~CPolygon()
{

}

//=============================================================
// [CPolygon] 初期化
//=============================================================
void CPolygon::Init()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	//デバイスの取得
	pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, nullptr);
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================
// [CPolygon] 終了
//=============================================================
void CPolygon::Uninit()
{
	//頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================
// [CPolygon] 更新
//=============================================================
void CPolygon::Update()
{
	UpdateAnim();
	UpdateVertex();
}

//=============================================================
// [CPolygon] 描画
//=============================================================
void CPolygon::DrawUI()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice; 
	pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数
}

//=============================================================
// [CPolygon] テクスチャの設定
//=============================================================
void CPolygon::SetTexture(std::string sPath)
{
	if (sPath != "")
	{
		CDataTexture* pTex = CDataManager::GetInstance()->RefTexture(sPath);

		if (pTex != nullptr)
		{
			m_pTexture = pTex->GetTexture();
		}
	}
	else
	{
		m_pTexture = nullptr;
	}
}

//=============================================================
// [CPolygon] 色の設定
//=============================================================
void CPolygon::SetColor(const D3DXCOLOR& color)
{
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	m_color = color;
}

//=============================================================
// [CPolygon] 色の設定
//=============================================================
void CPolygon::SetColor(const int& idx, const D3DXCOLOR& color)
{
	if (0 <= idx && idx < 4)
	{
		VERTEX_2D* pVtx; //頂点情報へのポインタ

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		// 頂点カラー
		pVtx[idx].col = color;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================
// [CPolygon] アニメーション設定
//=============================================================
void CPolygon::SetAnim(int nSplit, int nTime)
{
	if (nSplit > 0)
	{
		m_nSplit = nSplit;
		m_nTime = nTime;
		m_nextTime = timeGetTime() + m_nTime;
	}
}

//=============================================================
// [CPolygon] アニメーションの更新
//=============================================================
void CPolygon::UpdateAnim()
{
	VERTEX_2D* pVtx; //頂点情報へのポインタ
	float fSplitSize = 1.0f / (float)m_nSplit;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャ座標の設定
	if (!m_bReverse)
	{
		pVtx[0].tex = D3DXVECTOR2(fSplitSize * m_nAnimIdx, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fSplitSize * (m_nAnimIdx + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(fSplitSize * m_nAnimIdx, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fSplitSize * (m_nAnimIdx + 1), 1.0f);
	}
	else
	{
		pVtx[0].tex = D3DXVECTOR2(fSplitSize * (m_nAnimIdx + 1), 0.0f);
		pVtx[1].tex = D3DXVECTOR2(fSplitSize * m_nAnimIdx, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(fSplitSize * (m_nAnimIdx + 1), 1.0f);
		pVtx[3].tex = D3DXVECTOR2(fSplitSize * m_nAnimIdx, 1.0f);
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// アニメーションを進める
	if (timeGetTime() >= m_nextTime)
	{
		// 次のアニメーションに移る
		m_nAnimIdx++;

		// 次の時間を設定する
		m_nextTime = timeGetTime() + m_nTime;

		if (m_nAnimIdx >= m_nSplit)
		{ // アニメーションが最大数に達したとき
			m_nAnimIdx = 0;
		}
	}
}

//=============================================================
// [CPolygon] 頂点座標の更新
//=============================================================
void CPolygon::UpdateVertex()
{
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	D3DXMATRIX mtx;
	D3DXMatrixIdentity(&mtx);
	D3DXMatrixRotationZ(&mtx, transform->GetWRot().z);

	// 頂点位置を格納する
	D3DXVECTOR3 convPos[4] =
	{
		{0.0f, 0.0f, 0.0f},
		{transform->GetSize().x, 0.0f, 0.0f},
		{0.0f, transform->GetSize().y, 0.0f},
		{transform->GetSize().x, transform->GetSize().y, 0.0f}
	};

	// アンカーポイントを加味する
	for (int i = 0; i < 4; i++)
	{
		convPos[i].x -= m_anchorPoint.x;
		convPos[i].y -= m_anchorPoint.y;
	}


	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// サイズと回転
	D3DXVec3TransformCoord(&pVtx[0].pos, &convPos[0], &mtx);
	D3DXVec3TransformCoord(&pVtx[1].pos, &convPos[1], &mtx);
	D3DXVec3TransformCoord(&pVtx[2].pos, &convPos[2], &mtx);
	D3DXVec3TransformCoord(&pVtx[3].pos, &convPos[3], &mtx);

	// 位置
	for (int i = 0; i < 4; i++)
	{
		pVtx[i].pos.x += transform->GetWPos().x;
		pVtx[i].pos.y += transform->GetWPos().y;
	}

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}