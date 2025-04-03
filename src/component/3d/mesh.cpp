//=============================================================
//
// メッシュコンポーネント [mesh.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "mesh.h"
#include "manager.h"
#include <Shlwapi.h>								// ファイル存在チェックに使用
#pragma comment(lib, "Shlwapi.lib")			// ファイル存在チェックに使用

CCamera* CMesh::m_camera = nullptr;
const float CMesh::CAMERA_ALPHA_DISTANCE = 80.0f;

//=============================================================
// [CMesh] コンストラクタ
//=============================================================
CMesh::CMesh()
{
	m_pMesh = nullptr;
	m_pShadowMesh = nullptr;
	m_pBuffMat = nullptr;
	m_dwNumMat = 0;
	m_fAlpha = nullptr;
}

//=============================================================
// [CMesh] 引数付きコンストラクタ
//=============================================================
CMesh::CMesh(std::string sPath, bool bShadow) : CMesh()
{
	LoadMeshX(sPath, bShadow);
}

//=============================================================
// [CMesh] 初期化
//=============================================================
void CMesh::Init()
{
	
}

//=============================================================
// [CMesh] 終了
//=============================================================
void CMesh::Uninit()
{
	// ローカルマテリアルの破棄
	if (m_fAlpha != nullptr)
	{
		delete m_fAlpha;
		m_fAlpha = nullptr;
	}
}

//=============================================================
// [CMesh] 描画
//=============================================================
void CMesh::Draw()
{
	if (CShadow::USE_SHADOW)
	{ // 影使用時はスキップする
		return;
	}

	// 描画が必要か
	if (m_camera != nullptr)
	{
		// 半径を求める
		float maxRadius = Benlib::PosDistance({ 0.0f, 0.0f, 0.0f }, { m_pData->GetVertexRange().max.x * transform->GetWScale().x, m_pData->GetVertexRange().max.y * transform->GetWScale().y, m_pData->GetVertexRange().max.z * transform->GetWScale().z });
		float minRadius = Benlib::PosDistance({ 0.0f, 0.0f, 0.0f }, { m_pData->GetVertexRange().min.x * transform->GetWScale().x, m_pData->GetVertexRange().min.y * transform->GetWScale().y, m_pData->GetVertexRange().min.z * transform->GetWScale().z });
		float radius = maxRadius > minRadius ? maxRadius : minRadius;

		if (!Benlib::IsObjectInFrustum(m_camera->GetProjectionMatrix(), m_camera->GetViewMatrix(), transform->GetWPos(), radius))
		{ // 描画が必要ないとき
			return;
		}
	}

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();
	D3DMATERIAL9 matDef;					// 現在のマテリアル保存用
	D3DXMATERIAL* pMat;					// マテリアルデータへのポインタ

	if (m_pMesh == NULL || m_pBuffMat == NULL)
		return;

	// ワールドマトリックスの設定
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &transform->GetMatrix());

	// 現在のマテリアルを取得
	if (!IsEnabledShader()) pDevice->GetMaterial(&matDef);

	// マテリアルデータへのポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();

	Component::BeginPass();
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		// 不透明度
		float fAlpha, fBeforeAlpha;
		fBeforeAlpha = pMat[nCntMat].MatD3D.Diffuse.a;
		if (m_fAlpha != nullptr)
		{
			fAlpha = *m_fAlpha;
		}
		else
		{
			fAlpha = pMat[nCntMat].MatD3D.Diffuse.a;
		}

		// 色
		if (m_customColor)
		{
			pMat[nCntMat].MatD3D.Diffuse.r = m_color.r;
			pMat[nCntMat].MatD3D.Diffuse.g = m_color.g;
			pMat[nCntMat].MatD3D.Diffuse.b = m_color.b;
		}

		// マテリアルの設定
		pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;

		// カメラとの距離で透明度を特別に変更する
		if (m_camera != nullptr)
		{
			float distance = Benlib::PosDistance(m_camera->transform->GetWPos(), transform->GetWPos());

			// 透明範囲内の時
			if (distance < CAMERA_ALPHA_DISTANCE)
			{
				float alpha = distance / (float)CAMERA_ALPHA_DISTANCE - 0.05f;
				pMat[nCntMat].MatD3D.Diffuse.a = alpha;

				// Zバッファの書き込みを無効にする
				pDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
			}
		}

		// マテリアル適用
		if (!IsEnabledShader()) pDevice->SetMaterial(&pMat[nCntMat].MatD3D);

		// テクスチャの設定
		LPDIRECT3DTEXTURE9 tex;
		if (pMat[nCntMat].pTextureFilename != NULL)
		{ // テクスチャがあるとき
			tex = m_apTexture[nCntMat];
		}
		else
		{
			tex = NULL;
		}
		if (!IsEnabledShader()) pDevice->SetTexture(0, tex);

		// シェーダーに情報を渡す
		Shader::ParamData paramData;
		paramData.color = { pMat[nCntMat].MatD3D.Diffuse.r, pMat[nCntMat].MatD3D.Diffuse.g, pMat[nCntMat].MatD3D.Diffuse.b, pMat[nCntMat].MatD3D.Diffuse.a };
		paramData.texture = tex;
		paramData.mtx = transform->GetMatrix();
		Component::SetParam(paramData);

		// モデル（パーツ）の描画
		m_pMesh->DrawSubset(nCntMat);

		// Zバッファの書き込みを無効にする
		pDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);

		// マテリアル設定を戻す
		pMat[nCntMat].MatD3D.Diffuse.a = fBeforeAlpha;
	}
	Component::EndPass();

	// 保存していたマテリアルに戻す
	if (!IsEnabledShader()) pDevice->SetMaterial(&matDef);
}

//=============================================================
// [CMesh] Xファイルの読み込み
//=============================================================
HRESULT CMesh::LoadMeshX(std::string sPath, bool bShadow)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ファイルの存在チェック
	if (!PathFileExistsA(sPath.c_str())) return E_FAIL;

	// メッシュ情報を破棄
	Uninit();

	// メッシュを読み込む
	m_pData = CDataManager::GetInstance()->RefMesh(sPath);
	if (m_pData == nullptr)
	{ // 失敗
		return E_FAIL;
	}

	// メッシュのデータを参照する
	m_pMesh = m_pData->GetMesh();
	m_pBuffMat = m_pData->GetBuffMat();
	m_dwNumMat = m_pData->GetNumMat();

	// 影用メッシュの取得
	if (CShadow::USE_SHADOW && bShadow)
	{
		m_pShadowMesh = m_pData->GetShadowMesh();
	}

	// モデルテクスチャの読み込み
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	m_apTexture.resize(m_dwNumMat);
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{ // テクスチャがあるとき

			// テクスチャの存在チェック
			if (!PathFileExistsA(pMat[nCntMat].pTextureFilename))
			{ // 存在しないとき
				pMat[nCntMat].pTextureFilename = nullptr;
				m_apTexture[nCntMat] = nullptr;
				continue;
			}

			// テクスチャを作成
			m_apTexture[nCntMat] = CDataManager::GetInstance()->RefTexture(pMat[nCntMat].pTextureFilename)->GetTexture();
		}
		else
		{
			m_apTexture[nCntMat] = nullptr;
		}
	}

	return S_OK;
}

//=============================================================
// [CMesh] メッシュのセット
//=============================================================
HRESULT CMesh::SetMesh(LPD3DXMESH pMesh)
{
	// デバイスの取得
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// メッシュ情報を破棄
	Uninit();

	// メッシュのデータを参照する
	m_pMesh = pMesh;

	// モデルテクスチャの読み込み
	D3DXMATERIAL* pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	m_apTexture.resize(m_dwNumMat);
	for (int nCntMat = 0; nCntMat < (int)m_dwNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != nullptr)
		{ // テクスチャがあるとき

			// テクスチャの存在チェック
			if (!PathFileExistsA(pMat[nCntMat].pTextureFilename))
			{ // 存在しないとき
				pMat[nCntMat].pTextureFilename = nullptr;
				m_apTexture[nCntMat] = nullptr;
				continue;
			}

			// テクスチャを作成
			m_apTexture[nCntMat] = CDataManager::GetInstance()->RefTexture(pMat[nCntMat].pTextureFilename)->GetTexture();
		}
		else
		{
			m_apTexture[nCntMat] = nullptr;
		}
	}

	return S_OK;
}

//=============================================================
// [CMesh] テクスチャの設定
//=============================================================
void CMesh::SetTexture(int nIdx, LPDIRECT3DTEXTURE9 pTexture)
{
	if (0 <= nIdx && nIdx < (int)m_dwNumMat &&
		pTexture != nullptr)
	{ // インデックスが許容値のとき
		m_apTexture[nIdx] = pTexture;
	}
}

//=============================================================
// [CMesh] ローカルマテリアルの設定
//=============================================================
void CMesh::SetAlpha(const float& fAlpha)
{
	// まだ作成されていない場合は作成する
	if (m_fAlpha == nullptr)
	{
		m_fAlpha = new float;
	}

	// 設定を代入する
	*m_fAlpha = fAlpha;
}