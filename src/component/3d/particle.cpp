//=============================================================
//
// �p�[�e�B�N�� [particle.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "particle.h"
#include "renderer.h"
#include "internal/data_manager.h"

//=============================================================
// [Particle] ������
//=============================================================
void Particle::Init()
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_vtxBuff, NULL);
	VERTEX_3D* pVtx; //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//�@���̐ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	//���_�J���[
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//�e�N�X�`�����W�̐ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//���_�o�b�t�@���A�����b�N����
	m_vtxBuff->Unlock();
}

//=============================================================
// [Particle] �I��
//=============================================================
void Particle::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// [Particle] �X�V
//=============================================================
void Particle::Update()
{

}

//=============================================================
// [Particle] �`��
//=============================================================
void Particle::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// �f�o�C�X���擾
	D3DXMATRIX mtx = transform->GetMatrix();

	Component::BeginPass();

	// ���C�g�𖳌��ɂ���
	if (!IsEnabledShader()) pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̐ݒ�
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_texture);

	Shader::ParamData paramData;
	paramData.color = {1.0f, 1.0f, 1.0f, 1.0f};
	paramData.texture = m_texture;
	Component::SetParam(paramData);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0, //�`�悷��ŏ��̒��_�C���f�b�N�X
		2);				//�`�悷��v���~�e�B�u��

	Component::EndPass();

	// ���C�g��L���ɖ߂�
	if (!IsEnabledShader()) pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================
// [Particle] �T�C�Y�ύX
//=============================================================
void Particle::SetSize(const float& x, const float& y)
{
	VERTEX_3D* pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_vtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-x / 2, y / 2, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(x / 2, y / 2, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(-x / 2, -y / 2, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(x / 2, -y / 2, 0.0f);

	// ���_�o�b�t�@�̃A�����b�N
	m_vtxBuff->Unlock();
}

//=============================================================
// [Particle] �e�N�X�`���̐ݒ�
//=============================================================
void Particle::SetTexture(const std::string& path)
{
	BindTexture(CDataManager::GetInstance()->RefTexture(path)->GetTexture());
}