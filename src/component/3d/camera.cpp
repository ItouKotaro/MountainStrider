//=============================================================
//
// カメラコンポーネント [camera.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "camera.h"
#include "manager.h"

CCamera* CCamera::m_currentCamera = nullptr;

//=============================================================
// [CCamera] 初期化
//=============================================================
void CCamera::Init()
{
	m_fClippingPlanesNear = 0.3f;
	m_fClippingPlanesFar = 800.0f;
	m_backColor = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);
	m_vecU = { 0.0f, 1.0f, 0.0f };
	m_posR = { 0.0f, 0.0f, 0.0f };
	m_fLength = 120.0f;
	m_viewport.MinZ = 0.0f;
	m_viewport.MaxZ = 1.0f;
	SetViewportPosition(0.0f, 0.0f);
	SetViewportSize((float)CRenderer::SCREEN_WIDTH, (float)CRenderer::SCREEN_HEIGHT);
	m_bParallel = false;
	m_bVisible = true;
	m_bCustomPosR = false;
	m_fAmbient = 0.8f;

	// スカイボックスの初期化
	m_pSkybox = new CSkybox(transform);
	m_pSkybox->Init();
}

//=============================================================
// [CCamera] 終了
//=============================================================
void CCamera::Uninit()
{
	// スカイボックスを破棄する
	if (m_pSkybox != nullptr)
	{
		m_pSkybox->Uninit();
		delete m_pSkybox;
		m_pSkybox = nullptr;
	}
}

//=============================================================
// [CCamera] カメラの更新
//=============================================================
void CCamera::Update()
{
	if (!m_bCustomPosR)
	{
		D3DXMATRIX rotMtx = transform->GetRotationMatrix();
		m_posR = { 0.0f, 0.0f, m_fLength };
		D3DXVec3TransformCoord(&m_posR, &m_posR, &rotMtx);
		m_posR += transform->GetWPos();
	}

	// Z回転（上向きベクトル）
	//D3DXVECTOR3 defUp = { 0.0f, 1.0f, 0.0f };
	//D3DXMATRIX mtxUp;
	//D3DXMatrixIdentity(&mtxUp);
	//D3DXMatrixRotationZ(&mtxUp, transform->GetWRot().z);
	//D3DXMatrixInverse(&mtxUp, NULL, &mtxUp);
	//D3DXVec3TransformCoord(&m_vecU, &defUp, &mtxUp);
	//D3DXVec3Normalize(&m_vecU, &m_vecU);

	//m_vecU = { sinf(transform->GetWRot().z), cosf(transform->GetWRot().z), tanf(transform->GetWRot().z) };
	//D3DXVec3Normalize(&m_vecU, &m_vecU);
}


//=============================================================
// [CCamera] カメラの設定処理
//=============================================================
void CCamera::SetCamera(const bool& isSet)
{
	// デバイスを取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxProjection);

	D3DXVECTOR2 rect = CManager::GetInstance()->GetWindowSize();
	// プロジェクションマトリックスを作成
	if (!m_bParallel)
	{ // 透視投影
		D3DXMatrixPerspectiveFovLH(
			&m_mtxProjection,
			D3DXToRadian(45.0f),
			rect.x / rect.y,
			m_fClippingPlanesNear,
			m_fClippingPlanesFar);
	}
	else
	{ // 平行投影
		D3DXMatrixOrthoLH(
			&m_mtxProjection,
			rect.x,
			rect.y,
			m_fClippingPlanesNear,
			m_fClippingPlanesFar);

		// スケーリング調整
		D3DXMATRIX mtxScale;
		D3DXMatrixIdentity(&mtxScale);
		D3DXMatrixScaling(&mtxScale, 10.0f, 10.0f, 1.0f);	//	透視投影の場合Z軸が基本マイナスなのでfabsfを使用
		D3DXMatrixMultiply(&m_mtxProjection, &m_mtxProjection, &mtxScale);
	}


	// プロジェクションマトリックスの設定
	if (isSet)
		pDevice->SetTransform(D3DTS_PROJECTION, &m_mtxProjection);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxView);

	// ビューマトリックスの作成
	D3DXVECTOR3 pos = transform->GetWPos();
	D3DXMatrixLookAtLH(&m_mtxView, &pos, &m_posR, &m_vecU);

	// ビューマトリックスの設定
	if (isSet)
		pDevice->SetTransform(D3DTS_VIEW, &m_mtxView);

	// ビューポートの設定
	if (isSet)
		pDevice->SetViewport(&m_viewport);

	// 現在のカメラに設定する
	m_currentCamera = this;
}
