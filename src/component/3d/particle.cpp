//=============================================================
//
// パーティクル [particle.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "particle.h"
#include "renderer.h"
#include "internal/data_manager.h"

//=============================================================
// [Particle] 初期化
//=============================================================
void Particle::Init()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_vtxBuff, NULL);
	VERTEX_3D* pVtx; //頂点情報へのポインタ

	//頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	m_vtxBuff->Unlock();
}

//=============================================================
// [Particle] 終了
//=============================================================
void Particle::Uninit()
{
	// 頂点バッファの破棄
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// [Particle] 更新
//=============================================================
void Particle::Update()
{

}

//=============================================================
// [Particle] 描画
//=============================================================
void Particle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// デバイスを取得
	D3DXMATRIX mtx = transform->GetMatrix();

	Component::BeginPass();

	// ライトを無効にする
	if (!IsEnabledShader()) pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ワールドマトリックスの設定
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_3D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_3D);

	// テクスチャの設定
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_texture);

	Shader::ParamData paramData;
	paramData.color = {1.0f, 1.0f, 1.0f, 1.0f};
	paramData.texture = m_texture;
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
// [Particle] サイズ変更
//=============================================================
void Particle::SetSize(const float& x, const float& y)
{
	VERTEX_3D* pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(-x / 2, y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(x / 2, y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-x / 2, -y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(x / 2, -y / 2, 0.0f);

	// 頂点バッファのアンロック
	m_vtxBuff->Unlock();
}

//=============================================================
// [Particle] テクスチャの設定
//=============================================================
void Particle::SetTexture(const std::string& path)
{
	BindTexture(CDataManager::GetInstance()->RefTexture(path)->GetTexture());
}