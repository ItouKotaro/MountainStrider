//=============================================================
//
// �����_�[�e�N�X�`�� [render_texture.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "render_texture.h"
#include "manager.h"

//=============================================================
// [CRenderTexture] ������
//=============================================================
void CRenderTexture::Init()
{
	m_pTargetCamera = nullptr;
	m_pRenderTexture = nullptr;
	m_pTextureSurface = nullptr;
	m_pDepthSurface = nullptr;
}

//=============================================================
// [CRenderTexture] �I��
//=============================================================
void CRenderTexture::Uninit()
{
	// �e�N�X�`���̔j��
	if (m_pRenderTexture != nullptr)
	{
		m_pRenderTexture->Release();
		m_pRenderTexture = nullptr;
	}

	// �e�N�X�`���T�[�t�F�C�X�̔j��
	if (m_pTextureSurface != nullptr)
	{
		m_pTextureSurface->Release();
		m_pTextureSurface = nullptr;
	}

	// �[�x�o�b�t�@�̔j��
	if (m_pDepthSurface != nullptr)
	{
		m_pDepthSurface->Release();
		m_pDepthSurface = nullptr;
	}
}

//=============================================================
// [CRenderTexture] �R���|�[�l���g���j�����ꂽ�Ƃ��ɌĂяo����鏈��
//=============================================================
void CRenderTexture::OnDestroyOtherComponent(Component* other)
{
	if (m_pTargetCamera == other)
	{ // �J�����R���|�[�l���g�����X�g�����Ƃ�
		m_pTargetCamera = nullptr;
	}
}

//=============================================================
// [CRenderTexture] �����_�[�̊J�n
//=============================================================
bool CRenderTexture::BeginRender()
{
	// �ݒ�`�F�b�N
	if (!(m_pTargetCamera != nullptr &&
		m_pRenderTexture != nullptr &&
		m_pDepthSurface != nullptr))
	{
		return false;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �쐬�����X�e���V���o�b�t�@��ݒ肷��
	pDevice->SetDepthStencilSurface(m_pDepthSurface);

	// �쐬�����e�N�X�`���Ƀ����_�����O�^�[�Q�b�g��ύX
	LPDIRECT3DSURFACE9 pSurface;
	if (FAILED(m_pRenderTexture->GetSurfaceLevel(0, &pSurface)))
	{
		return false;
	}
	pDevice->SetRenderTarget(0, pSurface);

	// �L�����Ă���
	m_pTextureSurface = pSurface;

	// �V�[���̊J�n
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
// [CRenderTexture] �����_�[�̏I��
//=============================================================
void CRenderTexture::EndRender()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �V�[���̏I��
	pDevice->EndScene();
}

//=============================================================
// [CRenderTexture] �쐬
//=============================================================
HRESULT CRenderTexture::Create(const int& nW, const int& nH)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �j��
	this->Uninit();

	// �e�N�X�`���̍쐬
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

	// �[�x�o�b�t�@�̍쐬
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
// [CRenderTexture] �^�[�Q�b�g��ݒ肷��
//=============================================================
HRESULT CRenderTexture::SetTargetCamera(CCamera* pCamera)
{
	// ���݂��邩���m�F����
	if (Component::IsExist(pCamera))
	{ // ���݂���Ƃ�
		m_pTargetCamera = pCamera;
		return S_OK;
	}
	return E_FAIL;
}