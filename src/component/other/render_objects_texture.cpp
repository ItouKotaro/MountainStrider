//=============================================================
//
// �I�u�W�F�N�g�̃e�N�X�`�������_�[ [render_objects_texture.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "render_objects_texture.h"
#include "manager.h"

//=============================================================
// [CRenderObjectsTexture] ������
//=============================================================
void CRenderObjectsTexture::Init()
{
	m_pRenderTexture = nullptr;
	m_pTextureSurface = nullptr;
}

//=============================================================
// [CRenderObjectsTexture] �I��
//=============================================================
void CRenderObjectsTexture::Uninit()
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
}

//=============================================================
// [CRenderObjectsTexture] �쐬
//=============================================================
HRESULT CRenderObjectsTexture::Create(const int& nW, const int& nH)
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
	return S_OK;
}

//=============================================================
// [CRenderObjectsTexture] �I�u�W�F�N�g��ǉ�
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
// [CRenderObjectsTexture] �I�u�W�F�N�g���폜
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
// [CRenderObjectsTexture] �`�悷��
//=============================================================
void CRenderObjectsTexture::DrawTexture()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �쐬�����e�N�X�`���Ƀ����_�����O�^�[�Q�b�g��ύX
	LPDIRECT3DSURFACE9 pSurface;
	if (FAILED(m_pRenderTexture->GetSurfaceLevel(0, &pSurface)))
	{
		return;
	}
	pDevice->SetRenderTarget(0, pSurface);

	// �L�����Ă���
	m_pTextureSurface = pSurface;

	// �V�[���̊J�n
	if (FAILED(pDevice->BeginScene()))
	{
		return;
	}

	// ��ʃN���A�i�o�b�t�@�N���A��Z�o�b�t�@�N���A�j
	pDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	// �`��
	for (unsigned int i = 0; i < m_pRenderList.size(); i++)
	{
		if (m_pRenderList[i]->GetActive())
		{
			// �R���|�[�l���g�̃I�u�W�F�N�g�`�揈�����s��
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

	// �V�[���̏I��
	pDevice->EndScene();
}