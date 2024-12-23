//=============================================================
//
// �t���[���{�b�N�X [framebox.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "framebox.h"
#include "renderer.h"

//=============================================================
// [FrameBox] ������
//=============================================================
void FrameBox::Init()
{
	m_mainImg = new SingleComponent<CPolygon>();
	m_mainImg->Init();
	m_mainImg->SetParent(gameObject);

	m_maskImg = new SingleComponent<CPolygon>();
	m_maskImg->Init();
	m_maskImg->transform->SetParent(m_mainImg->transform);
}

//=============================================================
// [FrameBox] �I��
//=============================================================
void FrameBox::Uninit()
{
	if (m_mainImg != nullptr)
	{
		m_mainImg->Uninit();
		delete m_mainImg;
		m_mainImg = nullptr;
	}

	if (m_maskImg != nullptr)
	{
		m_maskImg->Uninit();
		delete m_maskImg;
		m_maskImg = nullptr;
	}
}

//=============================================================
// [FrameBox] �X�V
//=============================================================
void FrameBox::Update()
{
	// �T�C�Y����
	m_mainImg->transform->SetSize(transform->GetSize());
	m_maskImg->transform->SetSize(transform->GetSize().x - m_frameSize * 2, transform->GetSize().y - m_frameSize * 2);
	m_maskImg->transform->SetPos(m_frameSize, m_frameSize);

	m_mainImg->Update();
	m_maskImg->Update();
}

//=============================================================
// [FrameBox] �`��
//=============================================================
void FrameBox::DrawUI()
{
	// �f�o�C�X�̎擾
	auto pDevice = CRenderer::GetInstance()->GetDevice();

	pDevice->Clear(
		0,
		nullptr,
		D3DCLEAR_STENCIL,	// ����������o�b�t�@�̎��
		D3DCOLOR_ARGB(255, 255, 255, 0),				// �t���[���o�b�t�@�̏������F
		1.0f,							// Z�o�b�t�@�̏����l
		0);							// �X�e���V���o�b�t�@�̏����l

	// Z�o�b�t�@�̐ݒ��ύX����̂Ō��݂̏�Ԃ�ۑ����Ă���
	DWORD dwCurZTest, dwCurZFunc;
	pDevice->GetRenderState(D3DRS_ZENABLE, &dwCurZTest);
	pDevice->GetRenderState(D3DRS_ZFUNC, &dwCurZFunc);

	// Z�o�b�t�@�ɏ������܂Ȃ��悤�ɂ��܂�
	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_NEVER);

	// �X�e���V���o�b�t�@�̐ݒ�ł�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);  // �X�e���V���o�b�t�@�L��
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);
	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS); // �X�e���V���e�X�g�͏�ɍs��

	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_INCR);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_maskImg->DrawUI();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_mainImg->DrawUI();

	// ���Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, dwCurZTest);
	pDevice->SetRenderState(D3DRS_ZFUNC, dwCurZFunc);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x01);
}

//=============================================================
// [FrameBox] �e�N�X�`���̐ݒ�
//=============================================================
void FrameBox::SetTexture(const std::string& path)
{
	m_mainImg->SetTexture(path);
	m_maskImg->SetTexture(path);
}