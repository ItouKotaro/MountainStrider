//=============================================================
//
// ショップ [shop.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "shop.h"

#include "internal/data_manager.h"
#include "renderer.h"
#include "component/2d/text.h"

//=============================================================
// [ShopManager] 初期化
//=============================================================
void ShopManager::Init()
{
	m_shop = new GameObject();
	m_shop->SetVisible(false);

	// ポイント表示
	m_ptView = new GameObject();
	m_ptView->SetParent(m_shop);
	m_ptView->AddComponent<PointView>();
}

//=============================================================
// [ShopManager] 終了
//=============================================================
void ShopManager::Uninit()
{
}

//=============================================================
// [ShopManager] 更新
//=============================================================
void ShopManager::Update()
{
}

//=============================================================
// [ShopManager] 描画
//=============================================================
void ShopManager::Draw()
{
}




//=============================================================
// [PointView] 初期化
//=============================================================
void PointView::Init()
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスへのポインタ

	// デバイスの取得
	pDevice = CRenderer::GetInstance()->GetDevice();

	// テクスチャを読み込む
	m_texture = CDataManager::GetInstance()->RefTexture("data\\TEXTURE\\SHOP\\shop.png")->GetTexture();

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vtxBuff, nullptr);
	VERTEX_2D* pVtx; //頂点情報へのポインタ

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(CRenderer::SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, CRenderer::SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT, 0.0f);

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
	m_vtxBuff->Unlock();

}

//=============================================================
// [PointView] 終了
//=============================================================
void PointView::Uninit()
{
	// 頂点バッファの解放
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// [PointView] 更新
//=============================================================
void PointView::Update()
{

}

//=============================================================
// [PointView] 描画
//=============================================================
void PointView::DrawUI()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetInstance()->GetDevice();

	// 頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	pDevice->SetTexture(0, m_texture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //プリミティブの種類
		0, //描画する最初の頂点インデックス
		2); //描画するプリミティブ数
}