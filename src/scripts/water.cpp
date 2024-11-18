//=============================================================
//
// 水 [water.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "water.h"
#include "renderer.h"
#include "component/3d/field.h"

//=============================================================
// [CWater] コンストラクタ
//=============================================================
CWater::CWater()
{
    m_pEffect = NULL;

    for (int i = 0; i < 2; i++)
    {
        m_pWaveMapTexture[i] = NULL;
        m_pWaveMapSurface[i] = NULL;
    }

    m_pBumpMapTexture = NULL;
    m_pBumpMapSurface = NULL;

    m_RenderTargetIndex = 0;
}

//=============================================================
// [CWater] 初期化
//=============================================================
void CWater::Init()
{
    CPolygon::Init();

    m_pField = new GameObject;
    m_pField->AddComponent<CField>()->Set(1000.0f, 1000.0f);

    D3DCAPS9 caps;
    HRESULT hr;

    // デバイスの取得
    auto pDevice = CRenderer::GetInstance()->GetDevice();

    pDevice->GetDeviceCaps(&caps);
    if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
    {
        transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
        CPolygon::Update();

        //シェーダーの初期化
        LPD3DXBUFFER pErr = NULL;
        hr = D3DXCreateEffectFromFile(pDevice, "data\\SHADER\\Wave.fx", NULL, NULL, 0, NULL, &m_pEffect, &pErr);

        m_pTechnique = m_pEffect->GetTechniqueByName("TShader");
        m_pTexOffset = m_pEffect->GetParameterByName(NULL, "TexOffset");
        m_pSpringPower = m_pEffect->GetParameterByName(NULL, "SpringPower");
        m_pAddWavePos = m_pEffect->GetParameterByName(NULL, "AddWavePos");
        m_pAddWaveHeight = m_pEffect->GetParameterByName(NULL, "AddWaveHeight");

        m_pEffect->SetTechnique(m_pTechnique);

        D3DXVECTOR2 Size;
        Size.x = 1.0f / CRenderer::SCREEN_WIDTH;
        Size.y = 1.0f / CRenderer::SCREEN_HEIGHT;
        m_pEffect->SetFloatArray(m_pTexOffset, (float*)&Size, sizeof(D3DXVECTOR2));
    }
}

//=============================================================
// [CWater] 終了
//=============================================================
void CWater::Uninit()
{
    CPolygon::Uninit();

    // エフェクトの破棄
    if (m_pEffect != nullptr)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }

    // テクスチャの破棄
    for (int i = 0; i < 2; i++)
    {
        if (m_pWaveMapTexture[i] != nullptr)
        {
            m_pWaveMapTexture[i]->Release();
            m_pWaveMapTexture[i] = nullptr;
        }
        
        if (m_pWaveMapSurface[i] != nullptr)
        {
            m_pWaveMapSurface[i]->Release();
            m_pWaveMapSurface[i] = nullptr;
        }
    }

    if (m_pBumpMapTexture != nullptr)
    {
        m_pBumpMapTexture->Release();
        m_pBumpMapTexture = nullptr;
    }

    if (m_pBumpMapSurface != nullptr)
    {
        m_pBumpMapSurface->Release();
        m_pBumpMapSurface = nullptr;
    }
}

//=============================================================
// [CWater] 更新
//=============================================================
void CWater::Update()
{

}

//=============================================================
// [CWater] 描画
//=============================================================
void CWater::Draw()
{
    // デバイスの取得
    auto pDevice = CRenderer::GetInstance()->GetDevice();

    //****************************************************************
    // (STEP1)　波マップの更新
    //****************************************************************

    //浮動小数点フォーマットはビデオカードによっては「LINEAR」がきかないのでここでは「POINT」に設定しておく
    pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

    //2Dスプライトオブジェクトを使用してレンダリングするのでＺバッファを使用しない
    pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

    //波の移動速度
    SetSpringPower(0.2f);

    //適当に波を追加する
    AddWave((float)(rand() % 100) * 0.01f, (float)(rand() % 100) * 0.01f, (float)(rand() % 100 - 50) * 0.002f);

    //波マップの更新
    LPDIRECT3DTEXTURE9 BumpMap = DrawTexture();

    pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


    //****************************************************************
    // (STEP2)　背景をレンダリング
    //****************************************************************

    pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

    //水面
    m_pField->GetComponent<CField>()->BindTexture(BumpMap);
}

//=============================================================
// [CWater] テクスチャ描画
//=============================================================
LPDIRECT3DTEXTURE9 CWater::DrawTexture()
{
    if (m_pEffect)
    {
        D3DVIEWPORT9 OldViewport, NewViewport;

        // デバイスの取得
        auto pDevice = CRenderer::GetInstance()->GetDevice();

        // ビューポートを波マップのサイズに合わせる
        pDevice->GetViewport(&OldViewport);
        CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
        NewViewport.Width = (DWORD)CRenderer::SCREEN_WIDTH;
        NewViewport.Height = (DWORD)CRenderer::SCREEN_HEIGHT;
        pDevice->SetViewport(&NewViewport);

        //現在のレンダーターゲットサーフェイスを取得
        LPDIRECT3DSURFACE9 OldSurface = NULL;
        pDevice->GetRenderTarget(0, &OldSurface);

        m_RenderTargetIndex = 1 - m_RenderTargetIndex;

        //レンダーターゲットをセットする
        pDevice->SetRenderTarget(0, m_pWaveMapSurface[1 - m_RenderTargetIndex]);

        LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
        pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

        //デプスバッファを使用しないので無効にする(注意２)
        pDevice->SetDepthStencilSurface(NULL);

        //波マップをテクスチャーステージ０にセットする
        pDevice->SetTexture(0, m_pWaveMapTexture[m_RenderTargetIndex]);

        //波マップの更新
        m_pEffect->Begin(NULL, 0);
        m_pEffect->BeginPass(0);

        CPolygon::DrawUI();

        m_pEffect->EndPass();

        //更新した波マップを参照し水面を凸凹にしてレンダリングする
        pDevice->SetRenderTarget(0, m_pBumpMapSurface);

        pDevice->SetTexture(0, m_pWaveMapTexture[1 - m_RenderTargetIndex]);

        m_pEffect->BeginPass(1);

        CPolygon::DrawUI();

        m_pEffect->EndPass();
        m_pEffect->End();

        //戻す
        pDevice->SetRenderTarget(0, OldSurface);
        
        if (OldSurface != nullptr)
        {
            OldSurface->Release();
            OldSurface = nullptr;
        }

        pDevice->SetDepthStencilSurface(OldDepthStencilSurface);
        if (OldDepthStencilSurface != nullptr)
        {
            OldDepthStencilSurface->Release();
            OldDepthStencilSurface = nullptr;
        }

        pDevice->SetViewport(&OldViewport);
    }

    return m_pBumpMapTexture;
}

//=============================================================
// [CWater] スプリングパワーの設定
//=============================================================
void CWater::SetSpringPower(float SpringPower)
{
    if (m_pEffect)
    {
        m_pEffect->SetFloat(m_pSpringPower, SpringPower);
    }
}

//=============================================================
// [CWater] 波の追加
//=============================================================
void CWater::AddWave(float x, float y, float height)
{
    if (m_pEffect)
    {
        //波を追加する位置を設定
        D3DXVECTOR2 AddWavePos = D3DXVECTOR2(x, y);
        m_pEffect->SetValue(m_pAddWavePos, &AddWavePos, sizeof(D3DXVECTOR2));

        //追加する波の高さを設定する
        m_pEffect->SetFloat(m_pAddWaveHeight, height);
    }
}