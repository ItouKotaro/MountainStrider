//=============================================================
//
// オブジェクトのテクスチャレンダー [render_objects_texture.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "render_objects_texture.h"
#include "manager.h"

//=============================================================
// [CRenderObjectsTexture] 初期化
//=============================================================
void CRenderObjectsTexture::Init()
{
	m_pRenderTexture = nullptr;
	m_pTextureSurface = nullptr;
}

//=============================================================
// [CRenderObjectsTexture] 終了
//=============================================================
void CRenderObjectsTexture::Uninit()
{
	// テクスチャの破棄
	if (m_pRenderTexture != nullptr)
	{
		m_pRenderTexture->Release();
		m_pRenderTexture = nullptr;
	}

	// テクスチャサーフェイスの破棄
	if (m_pTextureSurface != nullptr)
	{
		m_pTextureSurface->Release();
		m_pTextureSurface = nullptr;
	}
}

//=============================================================
// [CRenderObjectsTexture] 作成
//=============================================================
HRESULT CRenderObjectsTexture::Create(const int& nW, const int& nH)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 破棄
	this->Uninit();

	// テクスチャの作成
	if (FAILED(D3DXCreateTexture(
		pDevice,
		nW,
		nH,
		0,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_R8G8B8,
		D3DPOOL_DEFAULT,
		&m_pRenderTexture)))
	{
		return E_FAIL;
	}
	return S_OK;
}

//=============================================================
// [CRenderObjectsTexture] オブジェクトを追加
//=============================================================
void CRenderObjectsTexture::AddObject(GameObject* pAddObj)
{
	bool bExist = false;
	for (unsigned int i = 0; i < m_pRenderList.size(); i++)
	{
		if (m_pRenderList[i] == pAddObj)
		{
			bExist = true;
			break;
		}
	}

	if (!bExist)
	{
		m_pRenderList.push_back(pAddObj);
	}
}

//=============================================================
// [CRenderObjectsTexture] オブジェクトを削除
//=============================================================
void CRenderObjectsTexture::RemoveObject(GameObject* pRemoveObj)
{
	for (unsigned int i = 0; i < m_pRenderList.size(); i++)
	{
		if (m_pRenderList[i] == pRemoveObj)
		{
			m_pRenderList.erase(m_pRenderList.begin() + i);
			break;
		}
	}
}

//=============================================================
// [CRenderObjectsTexture] 描画する
//=============================================================
void CRenderObjectsTexture::DrawTexture()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 作成したテクスチャにレンダリングターゲットを変更
	LPDIRECT3DSURFACE9 pSurface;
	if (FAILED(m_pRenderTexture->GetSurfaceLevel(0, &pSurface)))
	{
		return;
	}
	pDevice->SetRenderTarget(0, pSurface);

	// 記憶しておく
	m_pTextureSurface = pSurface;

	// シーンの開始
	if (FAILED(pDevice->BeginScene()))
	{
		return;
	}

	// 画面クリア（バッファクリア＆Zバッファクリア）
	pDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	// 描画
	for (unsigned int i = 0; i < m_pRenderList.size(); i++)
	{
		if (m_pRenderList[i]->GetActive())
		{
			// コンポーネントのオブジェクト描画処理を行う
			std::vector<Component*> pComponents = m_pRenderList[i]->GetComponents();
			for (int n = 0; n < (int)pComponents.size(); n++)
			{
				if (pComponents[n]->enabled)
				{
					pComponents[n]->Draw();
					pComponents[n]->DrawUI();
				}
			}
		}
	}

	// シーンの終了
	pDevice->EndScene();
}