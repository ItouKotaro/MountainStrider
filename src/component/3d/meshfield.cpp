//=============================================================
//
// メッシュフィールド [meshfield.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "meshfield.h"
#include "renderer.h"
#include "internal/data_manager.h"
#include "component/3d/line.h"

//=============================================================
// [CMeshField] 初期化
//=============================================================
void CMeshField::Init()
{
	// 変数の初期化
	m_sizeX = 0;
	m_sizeY = 0;
	m_sizeSpace = 0.0f;
	m_loopTex = 1;
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

	Component::BeginPass();

	// ワールドマトリックスの設定
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_pTexture);

	Shader::ParamData paramData;
	paramData.color = {1.0f, 1.0f, 1.0f, 1.0f};
	paramData.texture = m_pTexture;
	paramData.mtx = transform->GetMatrix();
	Component::SetParam(paramData);

	// ポリゴンの描画
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		(m_sizeX + 1) * (m_sizeY + 1),
		0,
		m_sizeX * m_sizeY * 2 + (m_sizeY - 1) * 4
	);

	Component::EndPass();
}

//=============================================================
// [CMeshField] テクスチャ設定
//=============================================================
void CMeshField::SetTexture(const std::string& sPath)
{
	BindTexture(CDataManager::GetInstance()->RefTexture(sPath)->GetTexture());
}

//=============================================================
// [CMeshField] テクスチャのループ数設定
//=============================================================
void CMeshField::SetLoopTexture(const int& num)
{
	if (num > 0)
	{
		m_loopTex = num;

		// 構成変数
		VERTEX_3D* pVtx;
		int nVertexLine = -1;	// 現在の列

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for (int nCntVertex = 0; nCntVertex < (m_sizeX + 1) * (m_sizeY + 1); nCntVertex++)
		{
			// 次の列に移動する
			if (nCntVertex % (m_sizeX + 1) == 0)
			{
				nVertexLine++; // 次の行に進める
			}

			pVtx[0].tex = D3DXVECTOR2((nCntVertex % (m_sizeX + 1)) / static_cast<float>((m_sizeX + 1)) * m_loopTex, (nVertexLine) / static_cast<float>((m_sizeY + 1)) * m_loopTex);

			pVtx++;
		}

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
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
	m_vertices.resize((x + 1) * (y + 1));

	// インデックスバッファの生成
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * ((2 * x + 2) * y + (y - 1) * 2 + 1),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
	m_indices.resize((2 * x + 2) * y + (y - 1) * 2 + 1);

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
		m_vertices[nCntVertex] = pVtx[0].pos;

		// 法線ベクトルの設定
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 頂点カラー
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2((nCntVertex % (x + 1)) / static_cast<float>(x), 1.0f - (nVertexLine) / static_cast<float>(y));

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
			pIdx[0] = static_cast<WORD>((x + nCntIdxWidth) + 1 + ((x + 1) * nCntIdxHeight));
			pIdx[1] = static_cast<WORD>(nCntIdxWidth + (x + 1) * nCntIdxHeight);

			m_indices[nCounter] = pIdx[0];
			m_indices[nCounter + 1] = pIdx[1];

			pIdx += 2;
			nCounter += 2;
		}

		// 折り返し
		pIdx[0] = static_cast<WORD>((x + 1) * (nCntIdxHeight + 1) - 1);
		m_indices[nCounter] = pIdx[0];
		pIdx += 1;
		nCounter++;

		if (nCntIdxHeight != y - 1)
		{
			pIdx[0] = static_cast<WORD>((x + 1) * (nCntIdxHeight + 2));
			m_indices[nCounter] = pIdx[0];
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
		m_vertices[nIndex] = pVtx->pos;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================
// [CMeshField] 色を変更する
//=============================================================
void CMeshField::SetColor(const int& x, const int& y, const D3DXCOLOR& color)
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
		pVtx->col = color;

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//=============================================================
// [CMeshField] 法線を正しい向きにリセットする
//=============================================================
void CMeshField::ResetNormals()
{
	// 頂点バッファをロック
	VERTEX_3D* pVtx;
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// インデックスバッファをロック
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// 頂点の法線をリセットする
	for (int i = 0; i < (m_sizeX + 1) * (m_sizeY + 1); i++)
	{
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	}

	// インデックスバッファを参照し法線の向きを求める
	for (int i = 0; i < (2 * m_sizeX + 2) * m_sizeY + (m_sizeY - 1) * 2 + 1; i+=3)
	{
		// インデックスを取得する
		int nIdx1 = static_cast<int>(pIdx[i]);
		int nIdx2 = static_cast<int>(pIdx[i+1]);
		int nIdx3 = static_cast<int>(pIdx[i+2]);

		// それぞれのインデックスの頂点座標を取得する
		D3DXVECTOR3 vtxPos1 = pVtx[nIdx1].pos;
		D3DXVECTOR3 vtxPos2 = pVtx[nIdx2].pos;
		D3DXVECTOR3 vtxPos3 = pVtx[nIdx3].pos;

		// 法線の方向を計算する
		D3DXVECTOR3 vec1 = vtxPos2 - vtxPos1;
		D3DXVECTOR3 vec2 = vtxPos3 - vtxPos1;
		D3DXVECTOR3 normal = { 0.0f, 0.0f, 0.0f };
		D3DXVec3Cross(&normal, &vec1, &vec2);
		D3DXVec3Normalize(&normal, &normal);
		normal = { fabsf(normal.x), fabsf(normal.y), fabsf(normal.z) };

		// 頂点の法線に加算する
		pVtx[nIdx1].nor += normal;
		pVtx[nIdx2].nor += normal;
		pVtx[nIdx3].nor += normal;
	}

	// 頂点の法線を正規化する
	for (int i = 0; i < (m_sizeX + 1) * (m_sizeY + 1); i++)
	{
		D3DXVECTOR3 nor = pVtx[i].nor;
		D3DXVec3Normalize(&nor, &nor);
		pVtx[i].nor = nor;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();

	// インデックスバッファをアンロック
	m_pIdxBuff->Unlock();
}

//=============================================================
// [CMeshField] 法線を表示する
//=============================================================
void CMeshField::ShowNormals(GameObject* pLineObj)
{
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVertex = 0; nCntVertex < (m_sizeX + 1) * (m_sizeY + 1); nCntVertex++)
	{
		pLineObj->AddComponent<CLine>()->SetLine(
			pVtx[0].pos + transform->GetWPos(),
			pVtx[0].pos + pVtx[0].nor * 15.0f + transform->GetWPos(),
			D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)
			);

		pVtx++;
	}

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}
