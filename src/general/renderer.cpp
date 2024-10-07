//=============================================================
//
// �����_���[ [renderer.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "renderer.h"
#include "gameobject.h"
#include "component/3d/camera.h"
#include "component/3d/render_texture.h"
#include "component/other/render_objects_texture.h"

// �ϐ��̏�����
const int CRenderer::SCREEN_WIDTH = 1920;
const int CRenderer::SCREEN_HEIGHT = 1080;
const bool CRenderer::USE_FOG = false;

//=============================================================
// [CRenderer] �R���X�g���N�^
//=============================================================
CRenderer::CRenderer()
{
	// �ϐ��̏�����
	m_pD3D = nullptr;
	m_pD3DDevice = nullptr;
	m_pShadow = nullptr;
	m_bFullScreen = true;
	m_fFogStartPos = 200.0f;
	m_fFogEndPos = 1000.0f;
}

//=============================================================
// [CRenderer] �f�X�g���N�^
//=============================================================
CRenderer::~CRenderer()
{

}

//=============================================================
// [CRenderer] ������
//=============================================================
HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;						// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;		// �v���[���e�[�V�����p�����[�^

	// Direct3D�I�u�W�F�N�g�̐���
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (m_pD3D == nullptr)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));														// �p�����[�^�̃[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;												// �Q�[����ʃT�C�Y�i���j
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;											// �Q�[����ʃT�C�Y�i�����j
	d3dpp.BackBufferFormat = d3ddm.Format;												// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;																	// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;									// �_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;													// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;									// �f�v�X�o�b�t�@
	d3dpp.Windowed = bWindow;																	// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;			// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;				// �C���^�[�o��

	// Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_�������n�[�h�E�F�A�ōs���j
	if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐����i�`�揈���̓n�[�h�E�F�A�Œ��_������CPU�ōs���j
		if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐����i�`�揈���ƒ��_������CPU�ōs���j
			if (FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				return E_FAIL;
			}
		}
	}

	// �n���h���̑��
	m_hwnd = hWnd;

	// �e�̐���
	if (CShadow::USE_SHADOW)
	{
		m_pShadow = new CShadow();
		if (!m_pShadow->CheckShadow(m_pD3D, m_pD3DDevice, d3dpp))
		{ // ���s
			return E_FAIL;
		}
		m_pShadow->Init();
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// �t�H�O
	if (USE_FOG)
	{
		m_pD3DDevice->SetRenderState(D3DRS_FOGENABLE, TRUE);
		m_pD3DDevice->SetRenderState(D3DRS_FOGCOLOR, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		m_pD3DDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);
		m_pD3DDevice->SetRenderState(D3DRS_FOGSTART, *(DWORD*)(&m_fFogStartPos));
		m_pD3DDevice->SetRenderState(D3DRS_FOGEND, *(DWORD*)(&m_fFogEndPos));
	}


	return S_OK;
}

//=============================================================
// [CRenderer] �I��
//=============================================================
void CRenderer::Uninit()
{
	// �e�̔j��
	if (m_pShadow != nullptr)
	{
		m_pShadow->Uninit();
		delete m_pShadow;
		m_pShadow = nullptr;
	}

	// Direct3D�f�o�C�X�̔j��
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}
}

//=============================================================
// [CRenderer] �X�V
//=============================================================
void CRenderer::Update()
{
	// ���ׂẴI�u�W�F�N�g���X�V����
	GameObject::UpdateAll();

	// �t���X�N���[���؂�ւ�
	if (INPUT_INSTANCE->onTrigger("f11"))
	{
		SetFullScreen(!GetFullScreen());
	}
}

//=============================================================
// [CRenderer] �`��
//=============================================================
void CRenderer::Draw()
{
	// �`��J�n
	if (SUCCEEDED(m_pD3DDevice->BeginScene()))
	{ // �`��J�n�����������ꍇ

		// ��ʃN���A�i�o�b�t�@�N���A��Z�o�b�t�@�N���A�j
		m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

		// �J�����̃R���|�[�l���g���擾����
		std::vector<CCamera*> cameraComponents = Component::GetComponents<CCamera>(false, true);

		for (unsigned int nCntCamera = 0; nCntCamera < cameraComponents.size(); nCntCamera++)
		{
			// �J�������擾
			CCamera* pCamera = cameraComponents.at(nCntCamera);

			if (!pCamera->GetVisible())
			{
				continue;	// �\�����Ȃ��Ƃ��͏������X�L�b�v
			}

			// ��ʃN���A�i�o�b�t�@�N���A��Z�o�b�t�@�N���A�j
			m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), pCamera->GetColor(), 1.0f, 0);

			// �J�������Z�b�g
			pCamera->SetCamera();
			pCamera->GetSkybox()->Draw();

			// �I�u�W�F�N�g�`��O����
			GameObject::BeforeDrawAll();

			// 3D�I�u�W�F�N�g�̕`��i�e�g�p���j
			if (CShadow::USE_SHADOW)
			{
				m_pShadow->Draw(pCamera);
			}

			// �Q�[���I�u�W�F�N�g�̕`�揈��
			GameObject::DrawAll();
		}

		// �Q�[���I�u�W�F�N�g��UI�`�揈��
		GameObject::DrawUIAll();

		// �`��I��
		m_pD3DDevice->EndScene();
	}

	// �����_�[�e�N�X�`�� --------------------------------------------------------------------------------------------------------------------

	// �R���|�[�l���g���擾����
	std::vector<CRenderTexture*> renderComponents = Component::GetComponents<CRenderTexture>(false, true);

	// �O��̐ݒ�Ƃ��ĕۑ����Ă���
	LPDIRECT3DSURFACE9 pBeforeBufferSurface;
	LPDIRECT3DSURFACE9 pBeforeDepthSurface;
	m_pD3DDevice->GetRenderTarget(0, &pBeforeBufferSurface);
	m_pD3DDevice->GetDepthStencilSurface(&pBeforeDepthSurface);

	for (unsigned int nCntRender = 0; nCntRender < renderComponents.size(); nCntRender++)
	{
		if (renderComponents[nCntRender]->BeginRender())
		{
			// �J�������擾
			CCamera* pCamera = renderComponents[nCntRender]->GetTargetCamera();

			// ��ʃN���A�i�o�b�t�@�N���A��Z�o�b�t�@�N���A�j
			m_pD3DDevice->Clear(0, nullptr, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), pCamera->GetColor(), 1.0f, 0);

			// �J�������Z�b�g
			pCamera->SetCamera();
			pCamera->GetSkybox()->Draw();

			// �I�u�W�F�N�g�`��O����
			GameObject::BeforeDrawAll();

			// 3D�I�u�W�F�N�g�̕`��i�e�g�p���j
			if (CShadow::USE_SHADOW)
			{
				m_pShadow->Draw(pCamera);
			}

			// �Q�[���I�u�W�F�N�g�̕`�揈��
			GameObject::DrawAll();

			renderComponents[nCntRender]->EndRender();
		}
	}

	// ���̐ݒ�ɖ߂�
	m_pD3DDevice->SetDepthStencilSurface(pBeforeDepthSurface);

	// �����_�[�I�u�W�F�N�g�e�N�X�`�� ----------------------------------------------------------------------------------------------------------

	// �R���|�[�l���g���擾����
	std::vector<CRenderObjectsTexture*> renderObjectsComponents = Component::GetComponents<CRenderObjectsTexture>(false, true);

	// �`��
	for (unsigned int nCntRender = 0; nCntRender < renderObjectsComponents.size(); nCntRender++)
	{
		renderObjectsComponents[nCntRender]->DrawTexture();
	}

	// ���̃����_�[�^�[�Q�b�g��ݒ肷��
	m_pD3DDevice->SetRenderTarget(0, pBeforeBufferSurface);

	// ------------------------------------------------------------------------------------------------------------------------------------

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	m_pD3DDevice->Present(nullptr, nullptr, nullptr, nullptr);
}

//=============================================================
// [CRenderer] �t���X�N���[��
//=============================================================
void CRenderer::SetFullScreen(const bool& bFullScreen)
{
	// �ϐ����㏑������
	m_bFullScreen = bFullScreen;

	// ���݂̃E�B���h�E�X�^�C�����擾
	DWORD dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);
	RECT rect = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }; // �E�B���h�E�̍��W���i�[

	// �t���X�N���[���ɐ؂�ւ���
	if (m_bFullScreen)
	{
		// �E�B���h�E���[�h�ɐ؂�ւ�
		SetWindowLong(m_hwnd, GWL_STYLE, dwStyle | WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hwnd, HWND_TOP, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hwnd, SW_NORMAL);
	}
	else
	{
		// �t���X�N���[�����[�h�ɐ؂�ւ�
		GetWindowRect(m_hwnd, &rect);
		SetWindowLong(m_hwnd, GWL_STYLE, dwStyle & ~WS_OVERLAPPEDWINDOW);
		SetWindowPos(m_hwnd, HWND_TOP, 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN),
			SWP_FRAMECHANGED | SWP_NOACTIVATE);
		ShowWindow(m_hwnd, SW_MAXIMIZE);
	}
}