//=============================================================
//
// �O�� [trajectory.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "trajectory.h"
#include "manager.h"

const float CTrajectory::FADE = 0.15f;
const char* CTrajectory::TEX_PATH = "data\\TEXTURE\\PARTICLE\\trajectory00.jpg";

//=============================================================
// [CTrajectory] ������
//=============================================================
void CTrajectory::Init()
{
	// �f�o�C�X���擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < MAX_FRAME; i++)
	{
		m_trajectoryData[i].rootPos = { 0.0f, 0.0f, 0.0f };
		m_trajectoryData[i].tipPos = { 0.0f, 0.0f, 0.0f };
	}

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, TEX_PATH, &m_pTextureTrajectory);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * (MAX_FRAME * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffTrajectory, NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(sizeof(WORD) * (2 * MAX_FRAME + 1),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuffTrajectory,
		nullptr);

	VERTEX_3D* pVtx;
	int nVertexLine = -1;	// ���݂̗�

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);

	int nCounter = 0;
	float fXTexSize = 1.0f / (float)MAX_FRAME;
	for (int nCntVertex = 0; nCntVertex < MAX_FRAME * 2; nCntVertex++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = D3DCOLOR_RGBA(200, 200, 200, 180);

		// �e�N�X�`�����W
		if (nCntVertex < MAX_FRAME)
		{
			pVtx[0].tex = D3DXVECTOR2(fXTexSize * (nCntVertex % MAX_FRAME), 0.0f);
		}
		else
		{
			pVtx[0].tex = D3DXVECTOR2(fXTexSize * (nCntVertex % MAX_FRAME), 1.0f);
		}

		pVtx++;
	}

	// �A�����b�N
	m_pVtxBuffTrajectory->Unlock();

	// �C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuffTrajectory->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	for (int nCntIdxWidth = 0; nCntIdxWidth < MAX_FRAME; nCntIdxWidth++)
	{
		pIdx[0] = (MAX_FRAME - 1 + nCntIdxWidth) + 1;
		pIdx[1] = nCntIdxWidth;

		pIdx += 2;
	}

	// �܂�Ԃ�
	pIdx[0] = (MAX_FRAME + 1) - 1;
	pIdx += 1;

	// �A�����b�N
	m_pIdxBuffTrajectory->Unlock();
}

//=============================================================
// [CTrajectory] �I��
//=============================================================
void CTrajectory::Uninit()
{
	// �e�N�X�`���̔j��
	if (m_pTextureTrajectory != nullptr)
	{
		m_pTextureTrajectory->Release();
		m_pTextureTrajectory = nullptr;
	}

	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuffTrajectory != nullptr)
	{
		m_pVtxBuffTrajectory->Release();
		m_pVtxBuffTrajectory = nullptr;
	}

	// �C���f�b�N�X�o�b�t�@�̔j��
	if (m_pIdxBuffTrajectory != nullptr)
	{
		m_pIdxBuffTrajectory->Release();
		m_pIdxBuffTrajectory = nullptr;
	}
}

//=============================================================
// [CTrajectory] �X�V
//=============================================================
void CTrajectory::Update()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();	// �f�o�C�X���擾

	VERTEX_3D* pVtx;
	int nVertexLine = -1;	// ���݂̗�

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuffTrajectory->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVertex = 0; nCntVertex < MAX_FRAME * 2; nCntVertex++)
	{
		// ���_���W�̐ݒ�
		if (nCntVertex < MAX_FRAME)
		{
			pVtx[0].pos = m_trajectoryData[nCntVertex % MAX_FRAME].tipPos;
		}
		else
		{
			pVtx[0].pos = m_trajectoryData[nCntVertex % MAX_FRAME].rootPos;
		}

		D3DXCOLOR col = pVtx[0].col;
		col.a = 1.0f - (nCntVertex % MAX_FRAME) * FADE;
		pVtx[0].col = col;

		pVtx++;
	}

	// �A�����b�N
	m_pVtxBuffTrajectory->Unlock();
}

//=============================================================
// [CTrajectory] �`��
//=============================================================
void CTrajectory::Draw()
{
	if (m_bShow == false)
	{
		return;
	}

	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// �f�o�C�X���擾
	D3DXMATRIX mtxRot, mtxTrans;							// �v�Z�p�}�g���b�N�X

	Component::BeginPass();

	//���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldTrajectory);

	// �ʒu�𔽉f
	D3DXMatrixTranslation(&m_mtxWorldTrajectory, 0.0f, 0.0f, 0.0f);

	// ���[���h�}�g���b�N�X�̐ݒ�
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorldTrajectory);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuffTrajectory, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuffTrajectory);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_pTextureTrajectory);

	// �|���S���`��O�ɐݒ�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	Shader::ParamData paramData;
	paramData.color = {1.0f, 1.0f, 1.0f, 1.0f};
	paramData.texture = m_pTextureTrajectory;
	paramData.mtx = transform->GetMatrix();
	Component::SetParam(paramData);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0,
		0,
		2 * (MAX_FRAME - 1) + 2,
		0,
		(MAX_FRAME - 1) * 2);

	Component::EndPass();

	// �|���S���`���Ɍ��ɖ߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	//���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//=============================================================
// [CTrajectory] �O���̒ǉ�
//=============================================================
void CTrajectory::AddTrajectory(const D3DXVECTOR3& root, const D3DXVECTOR3& tip)
{
	m_trajectoryData[MAX_FRAME - 1].rootPos = { 0.0f, 0.0f, 0.0f };
	m_trajectoryData[MAX_FRAME - 1].tipPos = { 0.0f, 0.0f, 0.0f };

	for (int i = 0; i < MAX_FRAME; i++)
	{
		if (MAX_FRAME - (2 + i) >= 0)
		{
			m_trajectoryData[MAX_FRAME - (1 + i)].rootPos = m_trajectoryData[MAX_FRAME - (2 + i)].rootPos;
			m_trajectoryData[MAX_FRAME - (1 + i)].tipPos = m_trajectoryData[MAX_FRAME - (2 + i)].tipPos;
		}
	}

	m_trajectoryData[0].rootPos = root;
	m_trajectoryData[0].tipPos = tip;
}

//=============================================================
// [CTrajectory] �\���ύX
//=============================================================
void CTrajectory::SetShow(const bool& bEnabled)
{
	m_bShow = bEnabled;

	if (bEnabled == false)
	{
		for (int i = 0; i < MAX_FRAME; i++)
		{
			m_trajectoryData[i].tipPos = { 0.0f, 0.0f, 0.0f };
			m_trajectoryData[i].rootPos = { 0.0f, 0.0f, 0.0f };
		}
	}
}

//=============================================================
// [CTrajectory] �O���ʒu�̃��Z�b�g
//=============================================================
void CTrajectory::ResetPos(const D3DXVECTOR3& pos)
{
	for (int i = 0; i < MAX_FRAME; i++)
	{
		m_trajectoryData[i].tipPos = pos;
		m_trajectoryData[i].rootPos = pos;
	}
}