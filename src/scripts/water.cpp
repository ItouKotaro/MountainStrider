//=============================================================
//
// �� [water.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "water.h"
#include "renderer.h"
#include "component/3d/field.h"

//=============================================================
// [CWater] �R���X�g���N�^
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
// [CWater] ������
//=============================================================
void CWater::Init()
{
    CPolygon::Init();

    m_pField = new GameObject;
    m_pField->AddComponent<CField>()->Set(1000.0f, 1000.0f);

    D3DCAPS9 caps;
    HRESULT hr;

    // �f�o�C�X�̎擾
    auto pDevice = CRenderer::GetInstance()->GetDevice();

    pDevice->GetDeviceCaps(&caps);
    if (caps.VertexShaderVersion >= D3DVS_VERSION(1, 1) && caps.PixelShaderVersion >= D3DPS_VERSION(2, 0))
    {
        transform->SetSize(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT);
        CPolygon::Update();

        //�V�F�[�_�[�̏�����
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
// [CWater] �I��
//=============================================================
void CWater::Uninit()
{
    CPolygon::Uninit();

    // �G�t�F�N�g�̔j��
    if (m_pEffect != nullptr)
    {
        m_pEffect->Release();
        m_pEffect = nullptr;
    }

    // �e�N�X�`���̔j��
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
// [CWater] �X�V
//=============================================================
void CWater::Update()
{

}

//=============================================================
// [CWater] �`��
//=============================================================
void CWater::Draw()
{
    // �f�o�C�X�̎擾
    auto pDevice = CRenderer::GetInstance()->GetDevice();

    //****************************************************************
    // (STEP1)�@�g�}�b�v�̍X�V
    //****************************************************************

    //���������_�t�H�[�}�b�g�̓r�f�I�J�[�h�ɂ���ẮuLINEAR�v�������Ȃ��̂ł����ł́uPOINT�v�ɐݒ肵�Ă���
    pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
    pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
    pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

    //2D�X�v���C�g�I�u�W�F�N�g���g�p���ă����_�����O����̂ły�o�b�t�@���g�p���Ȃ�
    pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);

    //�g�̈ړ����x
    SetSpringPower(0.2f);

    //�K���ɔg��ǉ�����
    AddWave((float)(rand() % 100) * 0.01f, (float)(rand() % 100) * 0.01f, (float)(rand() % 100 - 50) * 0.002f);

    //�g�}�b�v�̍X�V
    LPDIRECT3DTEXTURE9 BumpMap = DrawTexture();

    pDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);


    //****************************************************************
    // (STEP2)�@�w�i�������_�����O
    //****************************************************************

    pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
    pDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
    pDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

    //����
    m_pField->GetComponent<CField>()->BindTexture(BumpMap);
}

//=============================================================
// [CWater] �e�N�X�`���`��
//=============================================================
LPDIRECT3DTEXTURE9 CWater::DrawTexture()
{
    if (m_pEffect)
    {
        D3DVIEWPORT9 OldViewport, NewViewport;

        // �f�o�C�X�̎擾
        auto pDevice = CRenderer::GetInstance()->GetDevice();

        // �r���[�|�[�g��g�}�b�v�̃T�C�Y�ɍ��킹��
        pDevice->GetViewport(&OldViewport);
        CopyMemory(&NewViewport, &OldViewport, sizeof(D3DVIEWPORT9));
        NewViewport.Width = (DWORD)CRenderer::SCREEN_WIDTH;
        NewViewport.Height = (DWORD)CRenderer::SCREEN_HEIGHT;
        pDevice->SetViewport(&NewViewport);

        //���݂̃����_�[�^�[�Q�b�g�T�[�t�F�C�X���擾
        LPDIRECT3DSURFACE9 OldSurface = NULL;
        pDevice->GetRenderTarget(0, &OldSurface);

        m_RenderTargetIndex = 1 - m_RenderTargetIndex;

        //�����_�[�^�[�Q�b�g���Z�b�g����
        pDevice->SetRenderTarget(0, m_pWaveMapSurface[1 - m_RenderTargetIndex]);

        LPDIRECT3DSURFACE9 OldDepthStencilSurface = NULL;
        pDevice->GetDepthStencilSurface(&OldDepthStencilSurface);

        //�f�v�X�o�b�t�@���g�p���Ȃ��̂Ŗ����ɂ���(���ӂQ)
        pDevice->SetDepthStencilSurface(NULL);

        //�g�}�b�v���e�N�X�`���[�X�e�[�W�O�ɃZ�b�g����
        pDevice->SetTexture(0, m_pWaveMapTexture[m_RenderTargetIndex]);

        //�g�}�b�v�̍X�V
        m_pEffect->Begin(NULL, 0);
        m_pEffect->BeginPass(0);

        CPolygon::DrawUI();

        m_pEffect->EndPass();

        //�X�V�����g�}�b�v���Q�Ƃ����ʂ�ʉ��ɂ��ă����_�����O����
        pDevice->SetRenderTarget(0, m_pBumpMapSurface);

        pDevice->SetTexture(0, m_pWaveMapTexture[1 - m_RenderTargetIndex]);

        m_pEffect->BeginPass(1);

        CPolygon::DrawUI();

        m_pEffect->EndPass();
        m_pEffect->End();

        //�߂�
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
// [CWater] �X�v�����O�p���[�̐ݒ�
//=============================================================
void CWater::SetSpringPower(float SpringPower)
{
    if (m_pEffect)
    {
        m_pEffect->SetFloat(m_pSpringPower, SpringPower);
    }
}

//=============================================================
// [CWater] �g�̒ǉ�
//=============================================================
void CWater::AddWave(float x, float y, float height)
{
    if (m_pEffect)
    {
        //�g��ǉ�����ʒu��ݒ�
        D3DXVECTOR2 AddWavePos = D3DXVECTOR2(x, y);
        m_pEffect->SetValue(m_pAddWavePos, &AddWavePos, sizeof(D3DXVECTOR2));

        //�ǉ�����g�̍�����ݒ肷��
        m_pEffect->SetFloat(m_pAddWaveHeight, height);
    }
}