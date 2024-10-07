//=============================================================
//
// レンダーテクスチャ [render_texture.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "render_texture.h"
#include "manager.h"

//=============================================================
// [CRenderTexture] 初期化
//=============================================================
void CRenderTexture::Init()
{
	m_pTargetCamera = nullptr;
	m_pRenderTexture = nullptr;
	m_pTextureSurface = nullptr;
	m_pDepthSurface = nullptr;
}

//=============================================================
// [CRenderTexture] 終了
//=============================================================
void CRenderTexture::Uninit()
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

	// 深度バッファの破棄
	if (m_pDepthSurface != nullptr)
	{
		m_pDepthSurface->Release();
		m_pDepthSurface = nullptr;
	}
}

//=============================================================
// [CRenderTexture] コンポーネントが破棄されたときに呼び出される処理
//=============================================================
void CRenderTexture::OnDestroyOtherComponent(Component* other)
{
	if (m_pTargetCamera == other)
	{ // カメラコンポーネントをロストしたとき
		m_pTargetCamera = nullptr;
	}
}

//=============================================================
// [CRenderTexture] レンダーの開始
//=============================================================
bool CRenderTexture::BeginRender()
{
	// 設定チェック
	if (!(m_pTargetCamera != nullptr &&
		m_pRenderTexture != nullptr &&
		m_pDepthSurface != nullptr))
	{
		return false;
	}

	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// 作成したステンシルバッファを設定する
	pDevice->SetDepthStencilSurface(m_pDepthSurface);

	// 作成したテクスチャにレンダリングターゲットを変更
	LPDIRECT3DSURFACE9 pSurface;
	if (FAILED(m_pRenderTexture->GetSurfaceLevel(0, &pSurface)))
	{
		return false;
	}
	pDevice->SetRenderTarget(0, pSurface);

	// 記憶しておく
	m_pTextureSurface = pSurface;

	// シーンの開始
	if (SUCCEEDED(pDevice->BeginScene()))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//=============================================================
// [CRenderTexture] レンダーの終了
//=============================================================
void CRenderTexture::EndRender()
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// シーンの終了
	pDevice->EndScene();
}

//=============================================================
// [CRenderTexture] 作成
//=============================================================
HRESULT CRenderTexture::Create(const int& nW, const int& nH)
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

	// 深度バッファの作成
	if (FAILED(pDevice->CreateDepthStencilSurface(
		nW,
		nH,
		D3DFMT_D24S8,
		D3DMULTISAMPLE_NONE,
		0,
		TRUE,
		&m_pDepthSurface,
		NULL)))
	{
		return E_FAIL;
	}

	return S_OK;
}

//=============================================================
// [CRenderTexture] ターゲットを設定する
//=============================================================
HRESULT CRenderTexture::SetTargetCamera(CCamera* pCamera)
{
	// 存在するかを確認する
	if (Component::IsExist(pCamera))
	{ // 存在するとき
		m_pTargetCamera = pCamera;
		return S_OK;
	}
	return E_FAIL;
}