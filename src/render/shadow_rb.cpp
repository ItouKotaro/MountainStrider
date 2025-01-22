//=============================================================
//
// 影レンダー [shadow_rb.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shadow_rb.h"
#include "component/2d/polygon.h"
#include "component/3d/mesh.h"
#include "component/3d/meshfield.h"
#include "component/3d/particle.h"

const int ShadowRenderBuffer::SHADOWMAP_SIZE = 2048;

//=============================================================
// [ShadowMapShader] 初期化
//=============================================================
bool ShadowMapShader::Init()
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	// リソースにあるZ値プロットシェーダプログラムを読み込む
	D3DXCreateEffectFromFile(device,
		"data\\SHADER\\ShadowMap.cfx",
		nullptr,
		nullptr,
		D3DXSHADER_SKIPVALIDATION,
		nullptr,
		&m_effect,
		nullptr);

	m_hLightDiffuse = m_effect->GetParameterByName(NULL, "g_vLightDiffuse");
	m_hCosTheta = m_effect->GetParameterByName(NULL, "g_fCosTheta");
	m_hProj = m_effect->GetParameterByName(NULL, "g_mProj");
	m_hLightPos = m_effect->GetParameterByName(NULL, "g_vLightPos");
	m_hLightDir = m_effect->GetParameterByName(NULL, "g_vLightDir");
	m_hWorldView = m_effect->GetParameterByName(NULL, "g_mWorldView");
	m_hMaterial = m_effect->GetParameterByName(NULL, "g_vMaterial");
	m_hTexture = m_effect->GetParameterByName(NULL, "g_txScene");
	m_hShadowTex = m_effect->GetParameterByName(NULL, "g_txShadow");
	m_hViewToLightProj = m_effect->GetParameterByName(NULL, "g_mViewToLightProj");
	m_hLightAmbient = m_effect->GetParameterByName(NULL, "g_vLightAmbient");
	m_hRenderShadowTechnique = m_effect->GetTechniqueByName("RenderShadow");
	m_hRenderSceneTechnique = m_effect->GetTechniqueByName("RenderScene");
	m_hRenderLightTechnique = m_effect->GetTechniqueByName("RenderLight");

	// テクスチャの作成（白1pixel）
	device->CreateTexture(1, 1, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_defTex, NULL);
	D3DLOCKED_RECT lr;
	m_defTex->LockRect(0, &lr, NULL, 0);
	*(LPDWORD)lr.pBits = D3DCOLOR_RGBA(255, 255, 255, 255);
	m_defTex->UnlockRect(0);

	return true;
}

//=============================================================
// [ShadowMapShader] 終了
//=============================================================
void ShadowMapShader::Uninit()
{
	// テクスチャの破棄
	if (m_defTex != nullptr)
	{
		m_defTex->Release();
		m_defTex = nullptr;
	}
}

//=============================================================
// [ShadowMapShader] パラメーターの設定
//=============================================================
void ShadowMapShader::SetParam(const ParamData& data)
{
	// マテリアル設定
	D3DXVECTOR4 vDif = { data.color.r, data.color.g, data.color.b, data.color.a };
	m_effect->SetVector(m_hMaterial, &vDif);

	// テクスチャ設定
	if (data.texture != nullptr)
		m_effect->SetTexture(m_hTexture, data.texture);
	else
		m_effect->SetTexture(m_hTexture, m_defTex);


	D3DXMATRIXA16 mWorldView = data.mtx != nullptr ? data.mtx : data.component->transform->GetMatrix();
	D3DXMATRIX viewMtx = m_viewMtx;
	D3DXMatrixMultiply(&mWorldView, &mWorldView, &viewMtx);
	SetWorldView(&mWorldView);

	// 値の変更を適用
	HRESULT hr = m_effect->CommitChanges();
}

//=============================================================
// [ShadowRenderBuffer] 初期化
//=============================================================
void ShadowRenderBuffer::Init()
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	m_shadowPoint = { 0.0f, 0.0f, 0.0f };
	m_shadowRange = 600.0f;

	// シェーダーの初期化
	m_shader = new ShadowMapShader();
	m_shader->Init();

	// スポットライトの初期化
	m_lightFov = D3DXToRadian(45.0f);

	m_light.Diffuse.r = 1.0f;
	m_light.Diffuse.g = 1.0f;
	m_light.Diffuse.b = 1.0f;
	m_light.Diffuse.a = 1.0f;
	m_light.Position = D3DXVECTOR3(-8.0f, -8.0f, 0.0f);
	m_light.Direction = D3DXVECTOR3(1.0f, -1.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&m_light.Direction, (D3DXVECTOR3*)&m_light.Direction);
	m_light.Range = 20.0f;
	m_light.Theta = m_lightFov / 2.0f;
	m_light.Phi = m_lightFov / 2.0f;

	// シェーダーの値を設定する
	m_shader->SetLightDiffuse((D3DXVECTOR4*)&m_light.Diffuse);
	m_shader->SetCosTheta(cosf(m_light.Theta));

	// シャドウマップテクスチャを作成する
	if (FAILED( device->CreateTexture(
		SHADOWMAP_SIZE, SHADOWMAP_SIZE,
		1,
		D3DUSAGE_RENDERTARGET,
		D3DFMT_R32F,
		D3DPOOL_DEFAULT,
		&m_shadowMap,
		NULL
	)))
	{ // 失敗
		MessageBoxA(NULL, "シャドウマップテクスチャの作成に失敗！", "エラー", MB_OK);
	}

	// 深度バッファの作成
	//if (FAILED(device->CreateDepthStencilSurface(
	//	SHADOWMAP_SIZE, SHADOWMAP_SIZE,
	//	D3DFMT_D24S8,
	//	D3DMULTISAMPLE_NONE,
	//	0,
	//	TRUE,
	//	&m_dsShadow,
	//	NULL
	//)))
	//{ // 失敗
	//	MessageBoxA(NULL, "深度バッファの作成に失敗！", "エラー", MB_OK);
	//}

	//m_shadowMapTex = new GameObject();
	//m_shadowMapTex->AddComponent<CPolygon>()->BindTexture(m_shadowMap);
	//m_shadowMapTex->transform->SetSize(150, 150);
}

//=============================================================
// [ShadowRenderBuffer] 終了
//=============================================================
void ShadowRenderBuffer::Uninit()
{
	// シェーダーの解放
	if (m_shader != nullptr)
	{
		m_shader->Uninit();
		delete m_shader;
		m_shader = nullptr;
	}

	// シャドウマップテクスチャの破棄
	if (m_shadowMap != nullptr)
	{
		m_shadowMap->Release();
		m_shadowMap = nullptr;
	}

	// 深度バッファの破棄
	if (m_dsShadow != nullptr)
	{
		m_dsShadow->Release();
		m_dsShadow = nullptr;
	}
}

//=============================================================
// [ShadowRenderBuffer] 描画
//=============================================================
void ShadowRenderBuffer::Render()
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	if (m_camera == nullptr || m_lightCamera == nullptr)
	{
		CameraRenderBuffer::Render();
		return;
	}

	// カメラの更新
	m_camera->SetCamera(false);

	// ライトカメラの更新
	m_lightCamera->SetCamera(false);

	// ライトのビューマトリックスの取得
	D3DXMATRIX mLightView;
	mLightView = m_lightCamera->GetViewMatrix();

	// 影のプロジェクションマトリックス
	D3DXMatrixPerspectiveFovLH(&m_shadowProj, m_lightFov, 1, m_lightCamera->m_fClippingPlanesNear, m_lightCamera->m_fClippingPlanesFar);

	//----------------------------------------------------------------------------------
	// シャドウマップを描画する
	//----------------------------------------------------------------------------------
	LPDIRECT3DSURFACE9 pOldRT = NULL;
	device->GetRenderTarget(0, &pOldRT);
	LPDIRECT3DSURFACE9 pShadowSurf;
	if (SUCCEEDED(m_shadowMap->GetSurfaceLevel(0, &pShadowSurf)))
	{
		device->SetRenderTarget(0, pShadowSurf);
		if (pShadowSurf != nullptr)
		{
			pShadowSurf->Release();
			pShadowSurf = nullptr;
		}
	}
	LPDIRECT3DSURFACE9 pOldDS = NULL;
	if (SUCCEEDED(device->GetDepthStencilSurface(&pOldDS)))
		device->SetDepthStencilSurface(m_dsShadow);

	RenderScene(true, &mLightView, &m_shadowProj);

	if (pOldDS)
	{
		device->SetDepthStencilSurface(pOldDS);
		pOldDS->Release();
	}
	device->SetRenderTarget(0, pOldRT);
	if (pOldRT != nullptr)
	{
		pOldRT->Release();
		pOldRT = nullptr;
	}

	//----------------------------------------------------------------------------------
	// シーンの描画
	//----------------------------------------------------------------------------------
	D3DXMATRIX cameraViewMtx = m_camera->GetViewMatrix();
	const D3DXMATRIX* pmView = &cameraViewMtx;

	// 必要パラメーターの初期化
	m_shader->SetShadowTex(m_shadowMap);

	// ビュー空間から光投影空間に変換する
	D3DXMATRIXA16 mViewToLightProj;
	mViewToLightProj = *pmView;
	D3DXMatrixInverse(&mViewToLightProj, NULL, &mViewToLightProj);
	D3DXMatrixMultiply(&mViewToLightProj, &mViewToLightProj, &mLightView);
	D3DXMatrixMultiply(&mViewToLightProj, &mViewToLightProj, &m_shadowProj);
	m_shader->SetViewToLightProj(&mViewToLightProj);

	D3DXMATRIX cameraProjMtx = m_camera->GetProjectionMatrix();
	RenderScene(false, pmView, &cameraProjMtx);
	m_shader->SetShadowTex(NULL);
}

//=============================================================
// [ShadowRenderBuffer] シーン描画
//=============================================================
void ShadowRenderBuffer::RenderScene(bool renderShadow, const D3DXMATRIX* pmView, const D3DXMATRIX* pmProj)
{
	// デバイスの取得
	auto device = CRenderer::GetInstance()->GetDevice();

	// プロジェクションマトリックスを設定する
	m_shader->SetProj(pmProj);
	m_shader->SetViewMtx(*pmView);

	// ライトのパラメーターを更新する
	D3DXVECTOR3 v = *(D3DXVECTOR3*)&m_lightCamera->transform->GetMatrix()._41;
	D3DXVECTOR4 v4;
	D3DXVec3Transform(&v4, &v, pmView);
	m_shader->SetLightPos(&v4);
	*(D3DXVECTOR3*)&v4 = *(D3DXVECTOR3*)&m_lightCamera->transform->GetMatrix()._31;
	v4.w = 0.0f;
	D3DXVec4Transform(&v4, &v4, pmView);
	D3DXVec3Normalize((D3DXVECTOR3*)&v4, (D3DXVECTOR3*)&v4);
	m_shader->SetLightDir(&v4);

	// レンダーバッファをクリアする
	device->Clear(0L, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x000000ff, 1.0f, 0L);

	if (renderShadow)
		m_shader->SetRenderShadowTechnique();

	// 描画開始
	if (BeginScene())
	{
		if (!renderShadow)
		{
			m_shader->SetRenderSceneTechnique();

			m_camera->SetCamera();
			m_camera->GetSkybox()->Draw();
		}

		// オブジェクトを描画する
		auto allGameObjects = GameObject::GetAllGameObjects();

		// シェーダー開始
		m_shader->Begin();

		for (auto itr = allGameObjects.begin(); itr != allGameObjects.end(); itr++)
		{
			if ((*itr)->GetActive() && (*itr)->GetVisible())
			{ // アクティブのとき
				auto components = (*itr)->GetComponents();

				// コンポーネントの描画処理を行う
				for (auto itrComp = components.begin(); itrComp != components.end(); itrComp++)
				{
					if ((*itrComp)->enabled)
					{
						if (renderShadow && !Component::IsClassType<CMeshField>(*itrComp) &&
							Benlib::PosPlaneDistance((*itrComp)->transform->GetWPos(), m_shadowPoint) > m_shadowRange)
							break;

						// パーティクルは除外する
						if (renderShadow && Component::IsClassType<ParticleSystem>(*itrComp))
						{
							break;
						}

						// オブジェクトのマトリックスを設定する
						D3DXMATRIX mWorldView = (*itrComp)->transform->GetMatrix();
						D3DXMatrixMultiply(&mWorldView, &mWorldView, pmView);
						m_shader->SetWorldView(&mWorldView);

						// パス描画
						(*itrComp)->SetShader(m_shader, 0);
						(*itrComp)->Draw();

					}
				}
			}
		}

		// シェーダー終了
		m_shader->End();

		// UIの描画
		if (!renderShadow)
			GameObject::DrawUIAll();

		// シーンの終了
		EndScene();
	}

}