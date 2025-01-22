//=============================================================
//
// 影 [shadow.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "shadow.h"
#include "manager.h"
#include "component/3d/light.h"
#include "component/3d/mesh.h"
#include "component/3d/camera.h"
#include "component/3d/meshfield.h"
#include "benlib.h"
using namespace std;

// 変数の定義
const bool CShadow::USE_SHADOW = false;
const std::string CShadow::SHADOW_VOLUME_PATH = "./data/SHADER/ShadowVolume.cfx";		// シャドウボリュームのパス
const float CShadow::FAR_CLIP = 1000.0f;
const float CShadow::ADJACENCY_EPSILON = 0.0001f;
const float CShadow::EXTRUDE_EPSILON = 0.01f;
const CShadow::RENDER_TYPE CShadow::RENDER_MODE = CShadow::RENDERTYPE_SCENE;

// シャドウの頂点宣言
struct SHADOWVERT
{
    D3DXVECTOR3 Position;   // 位置
    D3DXVECTOR3 Normal;    // 法線
    const static D3DVERTEXELEMENT9 Decl[3];
};
const D3DVERTEXELEMENT9 SHADOWVERT::Decl[3] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    D3DDECL_END()
};

// メッシュの頂点宣言
struct MESHVERT
{
    D3DXVECTOR3 Position;
    D3DXVECTOR3 Normal;
    D3DXVECTOR2 Tex;
    const static D3DVERTEXELEMENT9 Decl[4];
};
const D3DVERTEXELEMENT9 MESHVERT::Decl[4] =
{
    { 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
    { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL,   0 },
    { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
    D3DDECL_END()
};

//=============================================================
// [CShadow] コンストラクタ
//=============================================================
CShadow::CShadow()
{
    m_pShadowVolume = nullptr;
    m_pMeshDecl = nullptr;
    m_pShadowDecl = nullptr;
    m_hShowShadow = nullptr;
    m_hRenderScene = nullptr;
    m_hRenderShadow = nullptr;
    m_pDefaultTex = nullptr;
    m_pLightMesh = nullptr;
}

//=============================================================
// [CShadow] デストラクタ
//=============================================================
CShadow::~CShadow()
{

}

//=============================================================
// [CShadow] 初期化
//=============================================================
void CShadow::Init()
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

    // エフェクトの初期設定
    m_pShadowVolume->SetFloat("g_fFarClip", FAR_CLIP);

    // ライト用のメッシュを作成する
    D3DXCreateSphere(pDevice, 1.0f, 32, 16, &m_pLightMesh, nullptr);
}

//=============================================================
// [CShadow] 終了
//=============================================================
void CShadow::Uninit()
{
    // エフェクトの破棄
    if (m_pShadowVolume != nullptr)
    {
        m_pShadowVolume->Release();
        m_pShadowVolume = nullptr;
    }

    // メッシュ宣言の破棄
    if (m_pMeshDecl != nullptr)
    {
        m_pMeshDecl->Release();
        m_pMeshDecl = nullptr;
    }

    // シャドウ宣言の破棄
    if (m_pShadowDecl != nullptr)
    {
        m_pShadowDecl->Release();
        m_pShadowDecl = nullptr;
    }

    // デフォルトテクスチャの破棄
    if (m_pDefaultTex != nullptr)
    {
        m_pDefaultTex->Release();
        m_pDefaultTex = nullptr;
    }
}

//=============================================================
// [CShadow] 影の描画
//=============================================================
void CShadow::Draw(CCamera* pCamera)
{
    D3DXMATRIX mtxProj = pCamera->GetProjectionMatrix();
    m_pShadowVolume->SetMatrix("g_mProj", &mtxProj);     // プロジェクションマトリックスを設定

    {
        m_pShadowVolume->SetTechnique("RenderSceneAmbient");
        D3DXVECTOR4 vAmb(pCamera->GetAmbient(), pCamera->GetAmbient(), pCamera->GetAmbient(), 1.0f);
        m_pShadowVolume->SetVector("g_vAmbient", &vAmb);
        // モデルの描画
        RenderMesh(pCamera, false);
    }

    // フォグが有効のとき、一時的にオフにする
    if (CRenderer::USE_FOG)
    {
        CRenderer::GetInstance()->GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
    }

    // 影の描画
    DrawShadow(pCamera);

    // フォグが有効のとき、オンに戻す
    if (CRenderer::USE_FOG)
    {
        CRenderer::GetInstance()->GetDevice()->SetRenderState(D3DRS_FOGENABLE, TRUE);
    }
}

//=============================================================
// [CShadow] 影の描画
//=============================================================
void CShadow::DrawShadow(CCamera* pCamera)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

    // ライトリストの取得
    vector<CLight*> pLightList = Component::GetComponents<CLight>(false, true);

    for (int nCntLight = 0; nCntLight < (int)pLightList.size(); nCntLight++)
    {
        // ライトの取得
        CLight* pLight = pLightList[nCntLight];

        // ステンシルバッファをクリアする
        pDevice->Clear(0, nullptr, D3DCLEAR_STENCIL, D3DCOLOR_ARGB(0, 170, 170, 170), 1.0f, 0);

        // ビューマトリックスとプロジェクションマトリックスを取得する
        D3DXMATRIX mtxView;
        mtxView = pCamera->GetViewMatrix();
        //mtxProj = pCamera->GetProjectionMatrix();

        // 照明の設定を適用する
        D3DXVECTOR3 lightPos = pLight->transform->GetWPos();
        D3DXVECTOR4 vLight(lightPos.x, lightPos.y, lightPos.z, 1.0f);
        D3DXVECTOR4 vLightColor = { pLight->GetColor().r, pLight->GetColor().g, pLight->GetColor().b, pLight->GetColor().a };
        vLightColor *= pLight->GetIntensity();

        D3DXVec4Transform(&vLight, &vLight, &mtxView);
        m_pShadowVolume->SetVector("g_vLightView", &vLight);
        m_pShadowVolume->SetVector("g_vLightColor", &vLightColor);

        // シャドウボリュームを描画する
        switch (RENDER_MODE)
        {
        case RENDERTYPE_SHADOWVOLUME:
            m_pShadowVolume->SetTechnique(m_hShowShadow);
            break;
        default:
            m_pShadowVolume->SetTechnique(m_hRenderShadow);
        }

        // シャドウボリュームの色を適用する（描画タイプがSHADOWVOLUMEのみ機能する）
        D3DXVECTOR4 volumeColor = { 
            pLight->GetVolumeColor().r,
            pLight->GetVolumeColor().g,
            pLight->GetVolumeColor().b,
            pLight->GetVolumeColor().a 
        };
        m_pShadowVolume->SetVector("g_vShadowColor", &volumeColor);

        // モデルリストを取得する
        std::vector<CMesh*> pMeshList = Component::GetComponents<CMesh>(false, true);

        for (unsigned int nCntModel = 0; nCntModel < pMeshList.size(); nCntModel++)
        {
            // モデルを取得する
            CMesh* pMesh = pMeshList[nCntModel];

            // モデルにシャドウメッシュがある場合のみ描画
            if (pMesh->GetShadowMesh() != nullptr)
            {
                // 影がジャギらないようにするための縮小スケール
                D3DXMATRIX mtxShadowScale;
                D3DXMatrixIdentity(&mtxShadowScale);
                D3DXMatrixScaling(&mtxShadowScale, 0.96f, 0.96f, 0.96f);

                // モデルのワールドマトリックスを適用する
                D3DXMATRIXA16 mWorldView = mtxShadowScale * pMesh->transform->GetMatrix() * pCamera->GetViewMatrix();
                D3DXMATRIXA16 mWorldViewProjection = mWorldView * pCamera->GetProjectionMatrix();
                m_pShadowVolume->SetMatrix("g_mWorldViewProjection", &mWorldViewProjection);
                m_pShadowVolume->SetMatrix("g_mWorldView", &mWorldView);
                pDevice->SetVertexDeclaration(m_pShadowDecl);

                // 影を描画する
                UINT cPasses;
                m_pShadowVolume->Begin(&cPasses, 0);
                for (UINT i = 0; i < cPasses; ++i)
                {
                    m_pShadowVolume->BeginPass(i);
                    m_pShadowVolume->CommitChanges();
                    pMesh->GetShadowMesh()->DrawSubset(0);
                    m_pShadowVolume->EndPass();
                }
                m_pShadowVolume->End();
            }
        }



        // ステンシルとライティングを有効にしてシーンをレンダリング
        m_pShadowVolume->SetTechnique(m_hRenderScene);
        RenderMesh(pCamera, false);
    }
}

//=============================================================
// [CShadow] メッシュのレンダー
//=============================================================
void CShadow::RenderMesh(CCamera* pCamera, bool bRenderLight)
{
    // デバイスの取得
    LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

    // マトリックス変数
    D3DXMATRIXA16 mWorldView;
    D3DXMATRIXA16 mViewProj;
    D3DXMATRIXA16 mWorldViewProjection;

    // 変数
    D3DXMATERIAL* pMat;					// マテリアルデータへのポインタ

    // ビューマトリックスとプロジェクションマトリックスを取得する
    D3DXMATRIX mtxView, mtxProj;
    mtxView = pCamera->GetViewMatrix();
    mtxProj = pCamera->GetProjectionMatrix();
    D3DXMatrixMultiply(&mViewProj, &mtxView, &mtxProj);

    // ライトを描画するか
    if (bRenderLight)
    { // 描画するとき

        D3DXHANDLE hCurrTech;
        hCurrTech = m_pShadowVolume->GetCurrentTechnique();  // Save the current technique
        m_pShadowVolume->SetTechnique("RenderSceneAmbient");
        m_pShadowVolume->SetTexture("g_txScene", m_pDefaultTex);
        D3DXVECTOR4 vLightMat(1.0f, 1.0f, 1.0f, 1.0f);
        m_pShadowVolume->SetVector("g_vMatColor", &vLightMat);
        UINT cPasses;
        m_pShadowVolume->Begin(&cPasses, 0);
        for (UINT p = 0; p < cPasses; ++p)
        {
            m_pShadowVolume->BeginPass(p);

            // ライトリストの取得
            vector<CLight*> pLightList = Component::GetComponents<CLight>(false, true);

            for (unsigned int i = 0; i < pLightList.size(); i++)
            {
                // ライトの取得
                CLight* pLight = pLightList[i];
                D3DXVECTOR4 lightCol = { 
                    pLight->GetColor().r,
                    pLight->GetColor().g,
                    pLight->GetColor().b,
                    pLight->GetColor().a 
                };
                lightCol *= pLight->GetIntensity();

                mWorldView = pLight->transform->GetMatrix() * pCamera->GetViewMatrix();
                mWorldViewProjection = mWorldView * pCamera->GetProjectionMatrix();
                m_pShadowVolume->SetMatrix("g_mWorldView", &mWorldView);
                m_pShadowVolume->SetMatrix("g_mWorldViewProjection", &mWorldViewProjection);
                m_pShadowVolume->SetVector("g_vAmbient", &lightCol);

                // The effect interface queues up the changes and performs them 
                // with the CommitChanges call. You do not need to call CommitChanges if 
                // you are not setting any parameters between the BeginPass and EndPass.
                m_pShadowVolume->CommitChanges();

                m_pLightMesh->DrawSubset(0);
            }
            m_pShadowVolume->EndPass();
        }
        m_pShadowVolume->End();
        m_pShadowVolume->SetTechnique(hCurrTech); // Restore the old technique
        D3DXVECTOR4 vAmb(pCamera->GetAmbient(), pCamera->GetAmbient(), pCamera->GetAmbient(), 1.0f);
        m_pShadowVolume->SetVector("g_vAmbient", &vAmb);
    }

    // メッシュリストを取得する
    vector<CMesh*> pMeshList = Component::GetComponents<CMesh>(false, true);
    
    for (unsigned int nCntModel = 0; nCntModel < pMeshList.size(); nCntModel++)
    {
        // モデルを取得する
        CMesh* pMesh = pMeshList[nCntModel];
        D3DXMATRIX modelMtx = pMesh->transform->GetMatrix();

        pDevice->SetVertexDeclaration(m_pMeshDecl);
        mWorldView = modelMtx * pCamera->GetViewMatrix();
        mWorldViewProjection = modelMtx * mViewProj;
        m_pShadowVolume->SetMatrix("g_mWorldViewProjection", &mWorldViewProjection);
        m_pShadowVolume->SetMatrix("g_mWorldView", &mWorldView);

        UINT cPasses;
        m_pShadowVolume->Begin(&cPasses, 0);
        for (UINT p = 0; p < cPasses; ++p)
        {
            m_pShadowVolume->BeginPass(p);

            pMat = (D3DXMATERIAL*)pMesh->GetBuffMat()->GetBufferPointer();
            for (int nCntMat = 0; nCntMat < (int)pMesh->GetNumMat(); nCntMat++)
            {
                // 不透明度
                float fAlpha, fBeforeAlpha;
                fBeforeAlpha = pMat[nCntMat].MatD3D.Diffuse.a;
                if (pMesh->GetAlpha() != nullptr)
                {
                    fAlpha = *pMesh->GetAlpha();
                }
                else
                {
                    fAlpha = pMat[nCntMat].MatD3D.Diffuse.a;
                }

                pMat[nCntMat].MatD3D.Diffuse.a = fAlpha;
                m_pShadowVolume->SetVector("g_vMatColor", (D3DXVECTOR4*)&pMat[nCntMat].MatD3D.Diffuse);

                if (pMesh->GetTextures()->at(nCntMat) != nullptr)
                { // テクスチャがあるとき
                    m_pShadowVolume->SetTexture("g_txScene", pMesh->GetTextures()->at(nCntMat));
                }
                else
                { // テクスチャがないとき
                    m_pShadowVolume->SetTexture("g_txScene", m_pDefaultTex);
                }
                m_pShadowVolume->CommitChanges();
                pMesh->GetMesh()->DrawSubset(nCntMat);

                // マテリアル設定を戻す
                pMat[nCntMat].MatD3D.Diffuse.a = fBeforeAlpha;
            }

            m_pShadowVolume->EndPass();
        }
        m_pShadowVolume->End();
    }
}

//=============================================================
// [CShadow] 能力チェック
//=============================================================
bool CShadow::CheckShadow(LPDIRECT3D9 pObject, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp)
{
    if (USE_SHADOW)
    {
        // 影の能力チェック
        if (!IsDeviceAcceptableShadow(pObject, pDevice, d3dpp))
        {
            return false;
        }

        // 影の頂点初期化
        InitVertexDeclarationShadow(pDevice);
    }
    return true;
}

//=============================================================
// デバイスの能力チェック
//=============================================================
bool CShadow::IsDeviceAcceptableShadow(LPDIRECT3D9 pObject, LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS d3dpp)
{
    // キャップを取得する
    D3DCAPS9 pCaps;
    pDevice->GetDeviceCaps(&pCaps);

    // アルファブレンディングをサポートしないバックバッファ形式をスキップ
    if (FAILED(pObject->CheckDeviceFormat(pCaps.AdapterOrdinal,
        pCaps.DeviceType,
        D3DFMT_X8R8G8B8,
        D3DUSAGE_QUERY_POSTPIXELSHADER_BLENDING,
        D3DRTYPE_TEXTURE,
        d3dpp.BackBufferFormat)))
    {
        return false;
    }

    // ピクセルシェーダ 2.0に対応しているか
    if (pCaps.PixelShaderVersion < D3DPS_VERSION(2, 0))
    {
        return false;
    }

    // ステンシルバッファに対応しているか
    if (!IsDepthFormatOk(D3DFMT_D24S8,
        D3DFMT_X8R8G8B8,
        d3dpp.BackBufferFormat,
        pObject))
    {
        return false;
    }

    return true;
}

//=============================================================
// 頂点宣言の初期化
//=============================================================
void CShadow::InitVertexDeclarationShadow(LPDIRECT3DDEVICE9 pDevice)
{
    // シャドウエフェクトファイルを読み込む
    DWORD dwShaderFlags = D3DXFX_NOT_CLONEABLE | D3DXSHADER_SKIPVALIDATION;
#if defined( _DEBUG )
    dwShaderFlags |= D3DXSHADER_DEBUG;
#endif
    D3DXCreateEffectFromFile(pDevice,
        SHADOW_VOLUME_PATH.c_str(),
        nullptr,
        nullptr,
        dwShaderFlags,
        nullptr,
        &m_pShadowVolume,
        nullptr);

    // 頂点宣言の初期化
    pDevice->CreateVertexDeclaration(MESHVERT::Decl, &m_pMeshDecl);
    pDevice->CreateVertexDeclaration(SHADOWVERT::Decl, &m_pShadowDecl);

    // デバイスの容量に基づいて、使用するレンダリング技術を決める
    D3DCAPS9 pCaps;
    pDevice->GetDeviceCaps(&pCaps); // キャップの取得

    m_hRenderScene = m_pShadowVolume->GetTechniqueByName("RenderScene");

    // 両面ステンシルがサポートされているか
    if (pCaps.StencilCaps & D3DSTENCILCAPS_TWOSIDED)
    { // サポートしている
        m_hRenderShadow = m_pShadowVolume->GetTechniqueByName("RenderShadowVolume2Sided");
        m_hShowShadow = m_pShadowVolume->GetTechniqueByName("ShowShadowVolume2Sided");
    }
    else
    { // サポートしていない
        m_hRenderShadow = m_pShadowVolume->GetTechniqueByName("RenderShadowVolume");
        m_hShowShadow = m_pShadowVolume->GetTechniqueByName("ShowShadowVolume");
    }

    // 1x1の白いテクスチャを作成する（デフォルトテクスチャ）
    pDevice->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8,
        D3DPOOL_MANAGED, &m_pDefaultTex, nullptr);
    D3DLOCKED_RECT lr;
    m_pDefaultTex->LockRect(0, &lr, nullptr, 0);
    *(LPDWORD)lr.pBits = D3DCOLOR_RGBA(255, 255, 255, 255);
    m_pDefaultTex->UnlockRect(0);
}

//=============================================================
// デプスフォーマットが対応しているかを調べる
//=============================================================
BOOL CShadow::IsDepthFormatOk(D3DFORMAT DepthFormat,
    D3DFORMAT AdapterFormat,
    D3DFORMAT BackBufferFormat,
    LPDIRECT3D9 pObject)
{
    // デプスフォーマットが存在することを確認する
    HRESULT hr = pObject->CheckDeviceFormat(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        AdapterFormat,
        D3DUSAGE_DEPTHSTENCIL,
        D3DRTYPE_SURFACE,
        DepthFormat);
    if (FAILED(hr)) return FALSE;

    // バックバッファのフォーマットが有効であることを確認する
    hr = pObject->CheckDeviceFormat(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        AdapterFormat,
        D3DUSAGE_RENDERTARGET,
        D3DRTYPE_SURFACE,
        BackBufferFormat);
    if (FAILED(hr)) return FALSE;

    // デプスフォーマットが互換性があることを確認する
    hr = pObject->CheckDepthStencilMatch(D3DADAPTER_DEFAULT,
        D3DDEVTYPE_HAL,
        AdapterFormat,
        BackBufferFormat,
        DepthFormat);

    return SUCCEEDED(hr);
}

//=============================================================
// [CShadow] 影メッシュの生成
//=============================================================
HRESULT CShadow::GenerateShadowMesh(IDirect3DDevice9* pDevice, ID3DXMesh* pMesh, ID3DXMesh** ppOutMesh)
{
    HRESULT hr = S_OK;
    ID3DXMesh* pInputMesh;

    if (!ppOutMesh)
        return E_INVALIDARG;
    *ppOutMesh = nullptr;

    // 入力メッシュを32bitインデックスを使って出力メッシュと同じフォーマットに変換する
    hr = pMesh->CloneMesh(D3DXMESH_32BIT, SHADOWVERT::Decl, pDevice, &pInputMesh);
    if (FAILED(hr))
        return hr;

    // 隣接情報の生成
    DWORD* pdwAdj = new DWORD[3 * pInputMesh->GetNumFaces()];
    DWORD* pdwPtRep = new DWORD[pInputMesh->GetNumVertices()];
    if (!pdwAdj || !pdwPtRep)
    {
        delete[] pdwAdj; delete[] pdwPtRep;
        pInputMesh->Release();
        return E_OUTOFMEMORY;
    }

    hr = pInputMesh->GenerateAdjacency(ADJACENCY_EPSILON, pdwAdj);
    if (FAILED(hr))
    {
        delete[] pdwAdj; delete[] pdwPtRep;
        pInputMesh->Release();
        return hr;
    }

    pInputMesh->ConvertAdjacencyToPointReps(pdwAdj, pdwPtRep);
    delete[] pdwAdj;

    SHADOWVERT* pVBData = nullptr;
    DWORD* pdwIBData = nullptr;

    pInputMesh->LockVertexBuffer(0, (LPVOID*)&pVBData);
    pInputMesh->LockIndexBuffer(0, (LPVOID*)&pdwIBData);

    if (pVBData && pdwIBData)
    {
        // エッジの最大数 = 面の数 * 3
        DWORD dwNumEdges = pInputMesh->GetNumFaces() * 3;
        CEdgeMapping* pMapping = new CEdgeMapping[dwNumEdges];
        if (pMapping)
        {
            int nNumMaps = 0;  // pMappingのエントリー数

            // メッシュを作成
            ID3DXMesh* pNewMesh;
            hr = D3DXCreateMesh(pInputMesh->GetNumFaces() + dwNumEdges * 2,
                pInputMesh->GetNumFaces() * 3,
                D3DXMESH_32BIT,
                SHADOWVERT::Decl,
                pDevice,
                &pNewMesh);
            if (SUCCEEDED(hr))
            {
                SHADOWVERT* pNewVBData = nullptr;
                DWORD* pdwNewIBData = nullptr;

                pNewMesh->LockVertexBuffer(0, (LPVOID*)&pNewVBData);
                pNewMesh->LockIndexBuffer(0, (LPVOID*)&pdwNewIBData);

                // 次の頂点インデックス値が格納されるIB内の配列インデックス
                int nNextIndex = 0;

                if (pNewVBData && pdwNewIBData)
                {
                    ZeroMemory(pNewVBData, pNewMesh->GetNumVertices() * pNewMesh->GetNumBytesPerVertex());
                    ZeroMemory(pdwNewIBData, sizeof(DWORD) * pNewMesh->GetNumFaces() * 3);

                    // 次の頂点を書き込む場所
                    SHADOWVERT* pNextOutVertex = pNewVBData;

                    // 新しいメッシュに新しい頂点と面を出力し、その辺をマッピング・テーブルに書き込む
                    for (UINT f = 0; f < pInputMesh->GetNumFaces(); ++f)
                    {
                        // 3頂点すべての頂点データをコピーする
                        CopyMemory(pNextOutVertex, pVBData + pdwIBData[f * 3], sizeof(SHADOWVERT));
                        CopyMemory(pNextOutVertex + 1, pVBData + pdwIBData[f * 3 + 1], sizeof(SHADOWVERT));
                        CopyMemory(pNextOutVertex + 2, pVBData + pdwIBData[f * 3 + 2], sizeof(SHADOWVERT));

                        // 面を書き出す
                        pdwNewIBData[nNextIndex++] = f * 3;
                        pdwNewIBData[nNextIndex++] = f * 3 + 1;
                        pdwNewIBData[nNextIndex++] = f * 3 + 2;

                        // 面の法線を計算し、それを頂点の法線に代入する
                        D3DXVECTOR3 v1, v2;
                        D3DXVECTOR3 vNormal;
                        v1 = *(D3DXVECTOR3*)(pNextOutVertex + 1) - *(D3DXVECTOR3*)pNextOutVertex;
                        v2 = *(D3DXVECTOR3*)(pNextOutVertex + 2) - *(D3DXVECTOR3*)(pNextOutVertex + 1);
                        D3DXVec3Cross(&vNormal, &v1, &v2);
                        D3DXVec3Normalize(&vNormal, &vNormal);

                        pNextOutVertex->Normal = vNormal;
                        (pNextOutVertex + 1)->Normal = vNormal;
                        (pNextOutVertex + 2)->Normal = vNormal;

                        pNextOutVertex += 3;

                        // 面のエッジをマッピングテーブルに追加する

                        // エッジ1
                        int nIndex;
                        int nVertIndex[3] =
                        {
                            static_cast<int>(pdwPtRep[pdwIBData[f * 3]]),
                            static_cast<int>(pdwPtRep[pdwIBData[f * 3 + 1]]),
                            static_cast<int>(pdwPtRep[pdwIBData[f * 3 + 2]])
                        };
                        nIndex = FindEdgeInMappingTable(nVertIndex[0], nVertIndex[1], pMapping, dwNumEdges);

                        // エラーの場合は中断
                        if (-1 == nIndex)
                        {
                            hr = E_INVALIDARG;
                            goto cleanup;
                        }

                        if (pMapping[nIndex].m_anOldEdge[0] == -1 && pMapping[nIndex].m_anOldEdge[1] == -1)
                        {
                            // エッジのエントリーがないので初期化
                            pMapping[nIndex].m_anOldEdge[0] = nVertIndex[0];
                            pMapping[nIndex].m_anOldEdge[1] = nVertIndex[1];
                            pMapping[nIndex].m_aanNewEdge[0][0] = f * 3;
                            pMapping[nIndex].m_aanNewEdge[0][1] = f * 3 + 1;

                            nNumMaps++;
                        }
                        else
                        {
                            // エッジのエントリーを見つけたので四角形を作成して出力する
                            assert(nNumMaps > 0);

                            pMapping[nIndex].m_aanNewEdge[1][0] = f * 3;
                            pMapping[nIndex].m_aanNewEdge[1][1] = f * 3 + 1;

                            // 1つ目の三角形
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];

                            // 2つ目の三角形
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];

                            // 最後のマップエントリーをコピーして、マップカウントをデクリメントする
                            pMapping[nIndex] = pMapping[nNumMaps - 1];
                            FillMemory(&pMapping[nNumMaps - 1], sizeof(pMapping[nNumMaps - 1]), 0xFF);
                            nNumMaps--;
                        }

                        // エッジ2
                        nIndex = FindEdgeInMappingTable(nVertIndex[1], nVertIndex[2], pMapping, dwNumEdges);

                        // エラーの場合は中断
                        if (-1 == nIndex)
                        {
                            hr = E_INVALIDARG;
                            goto cleanup;
                        }

                        if (pMapping[nIndex].m_anOldEdge[0] == -1 && pMapping[nIndex].m_anOldEdge[1] == -1)
                        {
                            pMapping[nIndex].m_anOldEdge[0] = nVertIndex[1];
                            pMapping[nIndex].m_anOldEdge[1] = nVertIndex[2];
                            pMapping[nIndex].m_aanNewEdge[0][0] = f * 3 + 1;
                            pMapping[nIndex].m_aanNewEdge[0][1] = f * 3 + 2;

                            nNumMaps++;
                        }
                        else
                        {
                            // エッジのエントリーを見つけたので四角形を作成して出力する
                            assert(nNumMaps > 0);

                            pMapping[nIndex].m_aanNewEdge[1][0] = f * 3 + 1;
                            pMapping[nIndex].m_aanNewEdge[1][1] = f * 3 + 2;

                            // 1つ目の三角形
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];

                            // 2つ目の三角形
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];

                            // 最後のマップエントリーをコピーして、マップカウントをデクリメントする
                            pMapping[nIndex] = pMapping[nNumMaps - 1];
                            FillMemory(&pMapping[nNumMaps - 1], sizeof(pMapping[nNumMaps - 1]), 0xFF);
                            nNumMaps--;
                        }

                        // エッジ3
                        nIndex = FindEdgeInMappingTable(nVertIndex[2], nVertIndex[0], pMapping, dwNumEdges);

                        // エラーの場合は中断
                        if (-1 == nIndex)
                        {
                            hr = E_INVALIDARG;
                            goto cleanup;
                        }

                        if (pMapping[nIndex].m_anOldEdge[0] == -1 && pMapping[nIndex].m_anOldEdge[1] == -1)
                        {
                            pMapping[nIndex].m_anOldEdge[0] = nVertIndex[2];
                            pMapping[nIndex].m_anOldEdge[1] = nVertIndex[0];
                            pMapping[nIndex].m_aanNewEdge[0][0] = f * 3 + 2;
                            pMapping[nIndex].m_aanNewEdge[0][1] = f * 3;

                            nNumMaps++;
                        }
                        else
                        {
                            // エッジのエントリーを見つけたので四角形を作成して出力する
                            assert(nNumMaps > 0);

                            pMapping[nIndex].m_aanNewEdge[1][0] = f * 3 + 2;
                            pMapping[nIndex].m_aanNewEdge[1][1] = f * 3;

                            // 1つ目の三角形
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];

                            // 2つ目の三角形
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][1];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[1][0];
                            pdwNewIBData[nNextIndex++] = pMapping[nIndex].m_aanNewEdge[0][0];

                            // 最後のマップエントリーをコピーして、マップカウントをデクリメントする
                            pMapping[nIndex] = pMapping[nNumMaps - 1];
                            FillMemory(&pMapping[nNumMaps - 1], sizeof(pMapping[nNumMaps - 1]), 0xFF);
                            nNumMaps--;
                        }
                    }


                    // 十分な大きさの頂点バッファとインデックスバッファを持つメッシュを作成

                    SHADOWVERT* pPatchVBData = nullptr;
                    DWORD* pdwPatchIBData = nullptr;

                    ID3DXMesh* pPatchMesh = nullptr;
                    // IBに十分なスペースを確保し、各パッチング面に最大3つの四角形を使用する
                    hr = D3DXCreateMesh(nNextIndex / 3 + nNumMaps * 7,
                        (pInputMesh->GetNumFaces() + nNumMaps) * 3,
                        D3DXMESH_32BIT,
                        SHADOWVERT::Decl,
                        pDevice,
                        &pPatchMesh);

                    if (FAILED(hr))
                        goto cleanup;

                    hr = pPatchMesh->LockVertexBuffer(0, (LPVOID*)&pPatchVBData);
                    if (SUCCEEDED(hr))
                        hr = pPatchMesh->LockIndexBuffer(0, (LPVOID*)&pdwPatchIBData);

                    if (pPatchVBData && pdwPatchIBData)
                    {
                        ZeroMemory(pPatchVBData, sizeof(SHADOWVERT) * (pInputMesh->GetNumFaces() + nNumMaps) *
                            3);
                        ZeroMemory(pdwPatchIBData, sizeof(DWORD) * (nNextIndex + 3 * nNumMaps * 7));

                        // 一方のメッシュから他のメッシュにデータをコピーする
                        CopyMemory(pPatchVBData, pNewVBData, sizeof(SHADOWVERT) * pInputMesh->GetNumFaces() * 3);
                        CopyMemory(pdwPatchIBData, pdwNewIBData, sizeof(DWORD) * nNextIndex);
                    }
                    else
                    {
                        // 重大なエラーによりロックできない場合
                        pPatchMesh->Release();
                        goto cleanup;
                    }

                    // 更新されたものに置き換える
                    pNewMesh->UnlockVertexBuffer();
                    pNewMesh->UnlockIndexBuffer();
                    pNewVBData = pPatchVBData;
                    pdwNewIBData = pdwPatchIBData;
                    pNewMesh->Release();
                    pNewMesh = pPatchMesh;

                    // 各共有エッジに対して四角形を生成する
                    // 非共有エッジごとに、新しい面で開口部をパッチする

                    // 次の頂点のインデックス
                    int nNextVertex = pInputMesh->GetNumFaces() * 3;

                    for (int i = 0; i < nNumMaps; ++i)
                    {
                        if (pMapping[i].m_anOldEdge[0] != -1 &&
                            pMapping[i].m_anOldEdge[1] != -1)
                        {
                            // 2番目の新しいエッジのインデックスが-1であれば、このエッジは非共有エッジである
                            if (pMapping[i].m_aanNewEdge[1][0] == -1 ||
                                pMapping[i].m_aanNewEdge[1][1] == -1)
                            {
                                // 現在の辺と頂点を共有する別の非共有辺を見つける
                                for (int i2 = i + 1; i2 < nNumMaps; ++i2)
                                {
                                    if (pMapping[i2].m_anOldEdge[0] != -1 &&
                                        pMapping[i2].m_anOldEdge[1] != -1 &&
                                        (pMapping[i2].m_aanNewEdge[1][0] == -1 ||
                                            pMapping[i2].m_aanNewEdge[1][1] == -1))
                                    {
                                        int nVertShared = 0;
                                        if (pMapping[i2].m_anOldEdge[0] == pMapping[i].m_anOldEdge[1])
                                            ++nVertShared;
                                        if (pMapping[i2].m_anOldEdge[1] == pMapping[i].m_anOldEdge[0])
                                            ++nVertShared;

                                        if (2 == nVertShared)
                                        {
                                            // 縮退四辺形を作成できるようにする
                                            pMapping[i2].m_aanNewEdge[1][0] = pMapping[i].m_aanNewEdge[0][0];
                                            pMapping[i2].m_aanNewEdge[1][1] = pMapping[i].m_aanNewEdge[0][1];
                                            break;
                                        }
                                        else if (1 == nVertShared)
                                        {
                                            // どちらのエッジがもう一方のエッジより前にあるか
                                            int nBefore, nAfter;
                                            if (pMapping[i2].m_anOldEdge[0] == pMapping[i].m_anOldEdge[1])
                                            {
                                                nBefore = i;
                                                nAfter = i2;
                                            }
                                            else
                                            {
                                                nBefore = i2;
                                                nAfter = i;
                                            }

                                            // 2つの辺から2つの退化した四角形と一緒に面を作る

                                            pNewVBData[nNextVertex] = pNewVBData[pMapping[nAfter].m_aanNewEdge[0][1]];
                                            pNewVBData[nNextVertex +
                                                1] = pNewVBData[pMapping[nBefore].m_aanNewEdge[0][1]];
                                            pNewVBData[nNextVertex +
                                                2] = pNewVBData[pMapping[nBefore].m_aanNewEdge[0][0]];

                                            // 法線を再計算する
                                            D3DXVECTOR3 v1 = pNewVBData[nNextVertex + 1].Position -
                                                pNewVBData[nNextVertex].Position;
                                            D3DXVECTOR3 v2 = pNewVBData[nNextVertex + 2].Position -
                                                pNewVBData[nNextVertex + 1].Position;
                                            D3DXVec3Normalize(&v1, &v1);
                                            D3DXVec3Normalize(&v2, &v2);
                                            D3DXVec3Cross(&pNewVBData[nNextVertex].Normal, &v1, &v2);
                                            pNewVBData[nNextVertex + 1].Normal = pNewVBData[nNextVertex +
                                                2].Normal = pNewVBData[nNextVertex].Normal;

                                            pdwNewIBData[nNextIndex] = nNextVertex;
                                            pdwNewIBData[nNextIndex + 1] = nNextVertex + 1;
                                            pdwNewIBData[nNextIndex + 2] = nNextVertex + 2;

                                            // 1つ目の四角形

                                            pdwNewIBData[nNextIndex + 3] = pMapping[nBefore].m_aanNewEdge[0][1];
                                            pdwNewIBData[nNextIndex + 4] = pMapping[nBefore].m_aanNewEdge[0][0];
                                            pdwNewIBData[nNextIndex + 5] = nNextVertex + 1;

                                            pdwNewIBData[nNextIndex + 6] = nNextVertex + 2;
                                            pdwNewIBData[nNextIndex + 7] = nNextVertex + 1;
                                            pdwNewIBData[nNextIndex + 8] = pMapping[nBefore].m_aanNewEdge[0][0];

                                            // 2つ目の四角形

                                            pdwNewIBData[nNextIndex + 9] = pMapping[nAfter].m_aanNewEdge[0][1];
                                            pdwNewIBData[nNextIndex + 10] = pMapping[nAfter].m_aanNewEdge[0][0];
                                            pdwNewIBData[nNextIndex + 11] = nNextVertex;

                                            pdwNewIBData[nNextIndex + 12] = nNextVertex + 1;
                                            pdwNewIBData[nNextIndex + 13] = nNextVertex;
                                            pdwNewIBData[nNextIndex + 14] = pMapping[nAfter].m_aanNewEdge[0][0];

                                            // 新しく追加された面の3番目のエッジを反映するように、i2を修正する

                                            if (pMapping[i2].m_anOldEdge[0] == pMapping[i].m_anOldEdge[1])
                                            {
                                                pMapping[i2].m_anOldEdge[0] = pMapping[i].m_anOldEdge[0];
                                            }
                                            else
                                            {
                                                pMapping[i2].m_anOldEdge[1] = pMapping[i].m_anOldEdge[1];
                                            }
                                            pMapping[i2].m_aanNewEdge[0][0] = nNextVertex + 2;
                                            pMapping[i2].m_aanNewEdge[0][1] = nNextVertex;

                                            // 次の頂点とインデックスの位置を更新する

                                            nNextVertex += 3;
                                            nNextIndex += 15;

                                            break;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                // 縮退した四角形を作る

                                // 1つ目の三角形
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[0][1];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[0][0];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[1][0];

                                // 2つ目の三角形
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[1][1];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[1][0];
                                pdwNewIBData[nNextIndex++] = pMapping[i].m_aanNewEdge[0][0];
                            }
                        }
                    }
                }

            cleanup:;
                if (pNewVBData)
                {
                    pNewMesh->UnlockVertexBuffer();
                    pNewVBData = nullptr;
                }
                if (pdwNewIBData)
                {
                    pNewMesh->UnlockIndexBuffer();
                    pdwNewIBData = nullptr;
                }

                if (SUCCEEDED(hr))
                {
                    // 出力されるメッシュのインデックスバッファは実際に必要なサイズより大きいかもしれないので
                    // 必要なIBサイズを正確に持つ別のメッシュを作成して出力する
                    // 32ビットが必要なければ16ビットのインデックスを使用する
                    bool bNeed32Bit = (pInputMesh->GetNumFaces() + nNumMaps) * 3 > 65535;
                    ID3DXMesh* pFinalMesh;
                    hr = D3DXCreateMesh(nNextIndex / 3,
                        (pInputMesh->GetNumFaces() + nNumMaps) * 3,
                        D3DXMESH_WRITEONLY | (bNeed32Bit ? D3DXMESH_32BIT : 0),
                        SHADOWVERT::Decl,
                        pDevice,
                        &pFinalMesh);
                    if (SUCCEEDED(hr))
                    {
                        pNewMesh->LockVertexBuffer(0, (LPVOID*)&pNewVBData);
                        pNewMesh->LockIndexBuffer(0, (LPVOID*)&pdwNewIBData);

                        SHADOWVERT* pFinalVBData = nullptr;
                        WORD* pwFinalIBData = nullptr;

                        pFinalMesh->LockVertexBuffer(0, (LPVOID*)&pFinalVBData);
                        pFinalMesh->LockIndexBuffer(0, (LPVOID*)&pwFinalIBData);

                        if (pNewVBData && pdwNewIBData && pFinalVBData && pwFinalIBData)
                        {
                            CopyMemory(pFinalVBData, pNewVBData, sizeof(SHADOWVERT) *
                                (pInputMesh->GetNumFaces() + nNumMaps) * 3);

                            if (bNeed32Bit)
                                CopyMemory(pwFinalIBData, pdwNewIBData, sizeof(DWORD) * nNextIndex);
                            else
                            {
                                for (int i = 0; i < nNextIndex; ++i)
                                    pwFinalIBData[i] = (WORD)pdwNewIBData[i];
                            }
                        }

                        if (pNewVBData)
                            pNewMesh->UnlockVertexBuffer();
                        if (pdwNewIBData)
                            pNewMesh->UnlockIndexBuffer();
                        if (pFinalVBData)
                            pFinalMesh->UnlockVertexBuffer();
                        if (pwFinalIBData)
                            pFinalMesh->UnlockIndexBuffer();

                        // 破棄する
                        pNewMesh->Release();
                        pNewMesh = pFinalMesh;
                    }

                    *ppOutMesh = pNewMesh;
                }
                else
                    pNewMesh->Release();
            }
            delete[] pMapping;
        }
        else
            hr = E_OUTOFMEMORY;
    }
    else
        hr = E_FAIL;

    if (pVBData)
        pInputMesh->UnlockVertexBuffer();

    if (pdwIBData)
        pInputMesh->UnlockIndexBuffer();

    delete[] pdwPtRep;
    pInputMesh->Release();

    return hr;
}

//=============================================================
// [CShadow] マッピングテーブルからエッジを探す処理
//=============================================================
int CShadow::FindEdgeInMappingTable(int nV1, int nV2, CEdgeMapping* pMapping, int nCount)
{
    for (int i = 0; i < nCount; ++i)
    {
        // マッピングエントリの古いエッジの頂点インデックスが両方とも -1 の場合、一致するエントリが見つからずに有効なエントリをすべて検索したことになる
        // このインデックスを新しく作成したエントリとして返す
        if ((pMapping[i].m_anOldEdge[0] == -1 && pMapping[i].m_anOldEdge[1] == -1) ||

            // 一致するものがあればインデックスを返す
            (pMapping[i].m_anOldEdge[1] == nV1 && pMapping[i].m_anOldEdge[0] == nV2))
        {
            return i;
        }
    }

    return -1;
}