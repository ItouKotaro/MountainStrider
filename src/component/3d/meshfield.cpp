//=============================================================
//
// メッシュフィールド [meshfield.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "meshfield.h"
#include "renderer.h"
#include "internal/data_manager.h"

//=============================================================
// [CMeshField] 初期化
//=============================================================
void CMeshField::Init()
{
	// 変数の初期化
	m_sizeX = 0;
	m_sizeY = 0;
	m_sizeSpace = 0.0f;
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pTexture = nullptr;
}

//=============================================================
// [CMeshField] 終了
//=============================================================
void CMeshField::Uninit()
{
	m_sizeX = 0;
	m_sizeY = 0;

	// 頂点バッファを破棄する
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// インデックスバッファを破棄する
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//=============================================================
// [CMeshField] 描画
//=============================================================
void CMeshField::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx = transform->GetMatrix();

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_pTexture);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		(m_sizeX + 1) * (m_sizeY + 1),
		0,
		((m_sizeX + 1) * 2) * (m_sizeY + 1)
	);
}

//=============================================================
// [CMeshField] テクスチャ設定
//=============================================================
void CMeshField::SetTexture(const std::string& sPath)
{
	BindTexture(CDataManager::GetInstance()->RefTexture(sPath)->GetTexture());
}

//=============================================================
// [CMeshField] 作成
//=============================================================
void CMeshField::Create(const int& x, const int& y, const float& spaceSize)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得

	// 終了
	this->Uninit();

	// 入力された数値が有効か
	if (!(x > 0 && y > 0))
	{
		return;	// 無効
	}

	m_sizeX = x;
	m_sizeY = y;
	m_sizeSpace = spaceSize;
		
	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * ((x + 1) * (y + 1)),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * ((2 * x + 2) * y + (y - 1) * 2 + 1),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// 構成変数
	VERTEX_3D* pVtx;
	int nVertexLine = -1;	// 現在の列

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVertex = 0; nCntVertex < (x + 1) * (y + 1); nCntVertex++)
	{
		// 次の列に移動する
		if (nCntVertex % (x + 1) == 0)
		{
			nVertexLine++; // 次の行に進める
		}

		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(
			spaceSize * (nCntVertex % (x + 1)) - (spaceSize * m_sizeX) / 2,
			0.0f,
			-spaceSize * nVertexLine + (spaceSize * m_sizeX) / 2
		);

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((nCntVertex % (x + 1)) / (x + 1), (nVertexLine) / (y + 1));

		pVtx++; // ポインタを進める
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// インデックスの設定
	int nCounter = 0;
	for (int nCntIdxHeight = 0; nCntIdxHeight < y; nCntIdxHeight++)
	{
		for (int nCntIdxWidth = 0; nCntIdxWidth < (x + 1); nCntIdxWidth++)
		{
			pIdx[0] = (x + nCntIdxWidth) + 1 + ((x + 1) * nCntIdxHeight);
			pIdx[1] = nCntIdxWidth + (x + 1) * nCntIdxHeight;

			pIdx += 2;
			nCounter += 2;
		}

		// 折り返し
		pIdx[0] = (x + 1) * (nCntIdxHeight + 1) - 1;
		pIdx += 1;
		nCounter++;

		if (nCntIdxHeight != y - 1)
		{
			pIdx[0] = (x + 1) * (nCntIdxHeight + 2);
			pIdx += 1;
			nCounter++;
		}
	}

	// インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
}

//=============================================================
// [CMeshField] 高さを変更する
//=============================================================
void CMeshField::SetHeight(const int& x, const int& y, const float& height)
{
	if (0 <= x && x <= m_sizeX &&
		0 <= y && y <= m_sizeY)
	{
		VERTEX_3D* pVtx;

		// インデックスを算出
		int nIndex = x + (m_sizeX + 1) * y;
		int nLine = (nIndex - nIndex % (m_sizeX + 1)) / (m_sizeX + 1);

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nIndex;

		// 指定の頂点を変更する
		D3DXVECTOR3 defPos = D3DXVECTOR3(
			m_sizeSpace * (nIndex % (m_sizeX + 1)) - (m_sizeSpace * m_sizeX) / 2,
			0.0f,
			-m_sizeSpace * nLine + (m_sizeSpace * m_sizeX) / 2
		);
		pVtx->pos = defPos + D3DXVECTOR3(0.0f, height, 0.0f);

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}
