//=============================================================
//
// ライン [line.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "line.h"
#include "manager.h"

//=============================================================
// [CLine] コンストラクタ
//=============================================================
CLine::CLine()
{
	m_start = { 0.0f, 0.0f, 0.0f };
	m_end = { 0.0f, 0.0f, 0.0f };
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
}

//=============================================================
// [CLine] 引数付きコンストラクタ
//=============================================================
CLine::CLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& color)
{
	m_start = start;
	m_end = end;
	m_color = color;
}

//=============================================================
// [CLine] デストラクタ
//=============================================================
CLine::~CLine()
{

}

//=============================================================
// [CLine] 初期化
//=============================================================
void CLine::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 2, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, nullptr);

	// ラインのセット
	SetLine(m_start, m_end, m_color);
}

//=============================================================
// [CLine] 終了
//=============================================================
void CLine::Uninit()
{
	// 破棄
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================
// [CLine] 更新
//=============================================================
void CLine::Update()
{

}

//=============================================================
// [CLine] 描画
//=============================================================
void CLine::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得

	// ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &transform->GetMatrix());

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	pDevice->SetTexture(0, nullptr);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_LINELIST, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		1);				//描画するプリミティブ数
}

//=============================================================
// [CLine] 設定
//=============================================================
void CLine::SetLine(const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXCOLOR& color)
{
	m_start = start;
	m_end = end;
	m_color = color;

	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = m_start;
	pVtx[1].pos = m_end;

	// 法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

	// 頂点カラー
	pVtx[0].col = m_color;
	pVtx[1].col = m_color;

	// テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);

	// 頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
}

//=============================================================
// [CLine] 色の設定
//=============================================================
void CLine::SetColor(const D3DXCOLOR& color)
{
	m_color = color;
}