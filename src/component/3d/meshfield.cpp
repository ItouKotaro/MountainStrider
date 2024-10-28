//=============================================================
//
// ���b�V���t�B�[���h [meshfield.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "meshfield.h"
#include "renderer.h"
#include "internal/data_manager.h"

//=============================================================
// [CMeshField] ������
//=============================================================
void CMeshField::Init()
{
	// �ϐ��̏�����
	m_sizeX = 0;
	m_sizeY = 0;
	m_sizeSpace = 0.0f;
	m_pVtxBuff = nullptr;
	m_pIdxBuff = nullptr;
	m_pTexture = nullptr;
}

//=============================================================
// [CMeshField] �I��
//=============================================================
void CMeshField::Uninit()
{
	m_sizeX = 0;
	m_sizeY = 0;

	// ���_�o�b�t�@��j������
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}

	// �C���f�b�N�X�o�b�t�@��j������
	if (m_pIdxBuff != nullptr)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = nullptr;
	}
}

//=============================================================
// [CMeshField] �`��
//=============================================================
void CMeshField::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// �f�o�C�X���擾
	D3DXMATRIX mtx = transform->GetMatrix();

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// �C���f�b�N�X�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetIndices(m_pIdxBuff);

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_pTexture);

	// �|���S���̕`��
	pDevice->DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		(m_sizeX + 1) * (m_sizeY + 1),
		0,
		((m_sizeX + 1) * 2) * (m_sizeY + 1)
	);
}

//=============================================================
// [CMeshField] �e�N�X�`���ݒ�
//=============================================================
void CMeshField::SetTexture(const std::string& sPath)
{
	BindTexture(CDataManager::GetInstance()->RefTexture(sPath)->GetTexture());
}

//=============================================================
// [CMeshField] �쐬
//=============================================================
void CMeshField::Create(const int& x, const int& y, const float& spaceSize)
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// �f�o�C�X���擾

	// �I��
	this->Uninit();

	// ���͂��ꂽ���l���L����
	if (!(x > 0 && y > 0))
	{
		return;	// ����
	}

	m_sizeX = x;
	m_sizeY = y;
	m_sizeSpace = spaceSize;
		
	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(
		sizeof(VERTEX_3D) * ((x + 1) * (y + 1)),
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);

	// �C���f�b�N�X�o�b�t�@�̐���
	pDevice->CreateIndexBuffer(
		sizeof(WORD) * ((2 * x + 2) * y + (y - 1) * 2 + 1),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);

	// �\���ϐ�
	VERTEX_3D* pVtx;
	int nVertexLine = -1;	// ���݂̗�

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntVertex = 0; nCntVertex < (x + 1) * (y + 1); nCntVertex++)
	{
		// ���̗�Ɉړ�����
		if (nCntVertex % (x + 1) == 0)
		{
			nVertexLine++; // ���̍s�ɐi�߂�
		}

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(
			spaceSize * (nCntVertex % (x + 1)) - (spaceSize * m_sizeX) / 2,
			0.0f,
			-spaceSize * nVertexLine + (spaceSize * m_sizeX) / 2
		);

		// �@���x�N�g���̐ݒ�
		pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// ���_�J���[
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2((nCntVertex % (x + 1)) / (x + 1), (nVertexLine) / (y + 1));

		pVtx++; // �|�C���^��i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();

	// �C���f�b�N�X�o�b�t�@�����b�N
	WORD* pIdx;
	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);

	// �C���f�b�N�X�̐ݒ�
	int nCounter = 0;
	for (int nCntIdxHeight = 0; nCntIdxHeight < y; nCntIdxHeight++)
	{
		for (int nCntIdxWidth = 0; nCntIdxWidth < (x + 1); nCntIdxWidth++)
		{
			pIdx[0] = (x + nCntIdxWidth) + 1 + ((x + 1) * nCntIdxHeight);
			pIdx[1] = nCntIdxWidth + (x + 1) * nCntIdxHeight;

			pIdx += 2;
			nCounter += 2;
		}

		// �܂�Ԃ�
		pIdx[0] = (x + 1) * (nCntIdxHeight + 1) - 1;
		pIdx += 1;
		nCounter++;

		if (nCntIdxHeight != y - 1)
		{
			pIdx[0] = (x + 1) * (nCntIdxHeight + 2);
			pIdx += 1;
			nCounter++;
		}
	}

	// �C���f�b�N�X�o�b�t�@���A�����b�N����
	m_pIdxBuff->Unlock();
}

//=============================================================
// [CMeshField] ������ύX����
//=============================================================
void CMeshField::SetHeight(const int& x, const int& y, const float& height)
{
	if (0 <= x && x <= m_sizeX &&
		0 <= y && y <= m_sizeY)
	{
		VERTEX_3D* pVtx;

		// �C���f�b�N�X���Z�o
		int nIndex = x + (m_sizeX + 1) * y;
		int nLine = (nIndex - nIndex % (m_sizeX + 1)) / (m_sizeX + 1);

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
		pVtx += nIndex;

		// �w��̒��_��ύX����
		D3DXVECTOR3 defPos = D3DXVECTOR3(
			m_sizeSpace * (nIndex % (m_sizeX + 1)) - (m_sizeSpace * m_sizeX) / 2,
			0.0f,
			-m_sizeSpace * nLine + (m_sizeSpace * m_sizeX) / 2
		);
		pVtx->pos = defPos + D3DXVECTOR3(0.0f, height, 0.0f);

		// ���_�o�b�t�@���A�����b�N����
		m_pVtxBuff->Unlock();
	}
}
