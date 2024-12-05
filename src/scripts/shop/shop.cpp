//=============================================================
//
// �V���b�v [shop.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "shop.h"

#include "internal/data_manager.h"
#include "renderer.h"
#include "component/2d/text.h"

//=============================================================
// [ShopManager] ������
//=============================================================
void ShopManager::Init()
{
	m_shop = new GameObject();
	m_shop->SetVisible(false);

	// �|�C���g�\��
	m_ptView = new GameObject();
	m_ptView->SetParent(m_shop);
	m_ptView->AddComponent<PointView>();
}

//=============================================================
// [ShopManager] �I��
//=============================================================
void ShopManager::Uninit()
{
}

//=============================================================
// [ShopManager] �X�V
//=============================================================
void ShopManager::Update()
{
}

//=============================================================
// [ShopManager] �`��
//=============================================================
void ShopManager::Draw()
{
}




//=============================================================
// [PointView] ������
//=============================================================
void PointView::Init()
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = CRenderer::GetInstance()->GetDevice();

	// �e�N�X�`����ǂݍ���
	m_texture = CDataManager::GetInstance()->RefTexture("data\\TEXTURE\\SHOP\\shop.png")->GetTexture();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vtxBuff, nullptr);
	VERTEX_2D* pVtx; //���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(CRenderer::SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, CRenderer::SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(CRenderer::SCREEN_WIDTH, CRenderer::SCREEN_HEIGHT, 0.0f);

	// rhw�̐ݒ�
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	// ���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	// �e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	// ���_�o�b�t�@���A�����b�N����
	m_vtxBuff->Unlock();

}

//=============================================================
// [PointView] �I��
//=============================================================
void PointView::Uninit()
{
	// ���_�o�b�t�@�̉��
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// [PointView] �X�V
//=============================================================
void PointView::Update()
{

}

//=============================================================
// [PointView] �`��
//=============================================================
void PointView::DrawUI()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetInstance()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_texture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0, //�`�悷��ŏ��̒��_�C���f�b�N�X
		2); //�`�悷��v���~�e�B�u��
}