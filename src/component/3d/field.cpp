//=============================================================
//
// フィールドコンポーネント [field.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "field.h"
#include "manager.h"

//=============================================================
// [CField] 初期化
//=============================================================
void CField::Init()
{
	// 変数の初期化
	m_pVtxBuff = nullptr;											// 頂点バッファ
	m_pTexture = nullptr;										// テクスチャ
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// カラー

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
// [CField] 終了
//=============================================================
void CField::Uninit()
{
	// 頂点バッファの破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================
// [CField] 描画
//=============================================================
void CField::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx = transform->GetMatrix();

	Component::BeginPass();

	// ライトを無効にする
	if (!IsEnabledShader()) pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの設定
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_pTexture);

	Shader::ParamData paramData;
	paramData.color = m_color;
	paramData.texture = m_pTexture;
	paramData.mtx = transform->GetMatrix();
	Component::SetParam(paramData);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2);				//描画するプリミティブ数

	Component::EndPass();

	// ライトを有効に戻す
	if (!IsEnabledShader()) pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================
// [CField] テクスチャをセットする
//=============================================================
void CField::SetTexture(const std::string& sPath)
{
	BindTexture(CDataManager::GetInstance()->RefTexture(sPath)->GetTexture());
}

//=============================================================
// [CField] 初期化
//=============================================================
void CField::Set(float fWidth, float fDepth)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-fWidth * 0.5f, 0.0f, fDepth * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(fWidth * 0.5f, 0.0f, fDepth * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-fWidth * 0.5f, 0.0f, -fDepth * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(fWidth * 0.5f, 0.0f, -fDepth * 0.5f);

	//法線の設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	//頂点カラー
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================
// [CField] 色
//=============================================================
void CField::SetColor(D3DXCOLOR col)
{
	m_color = col;

	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラー
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================
// [CField] テクスチャのループ数設定
//=============================================================
void CField::SetLoopTexture(const int& num)
{
	if (num > 0)
	{
		// 構成変数
		VERTEX_3D* pVtx;
		int nVertexLine = -1;	// 現在の列

		//頂点バッファをロックし、頂点情報へのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		//テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(static_cast<float>(num), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, static_cast<float>(num));
		pVtx[3].tex = D3DXVECTOR2(static_cast<float>(num), static_cast<float>(num));

		// 頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}