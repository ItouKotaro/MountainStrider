//=============================================================
//
// ���U���g�̒n�`�\�� [result_terrain.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "result_terrain.h"
#include "renderer.h"
#include "scene/game.h"
#include "scripts/terrain.h"

const std::string ResultTerrain::TERRAIN_TEX = "data\\terrain.bmp";
const int ResultTerrain::SIZE = 500.0f;
const float ResultTerrain::TRAVELLING_POINT_SIZE = 5.0f;
const int ResultTerrain::TRAVELLING_FRAME = 2;
const int ResultTerrain::TRAVELLING_ENDFRAME = 120;
//=============================================================
// [ResultTerrain] ������
//=============================================================
void ResultTerrain::Init()
{
	m_terrainVtxBuff = nullptr;
	m_texture = nullptr;
	m_travellingCounter = 0;

	// ���s�f�[�^���擾����
	auto travellingData = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTravellingData();
	if (travellingData.size() <= 0)
	{
		return;
	}


	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// �e�N�X�`���̐���
	D3DXCreateTextureFromFile(pDevice, TERRAIN_TEX.c_str(), &m_texture);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_terrainVtxBuff, nullptr);
	VERTEX_2D* pVtx; //���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_terrainVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(SIZE * -0.5f, SIZE * -0.5f, 0.0f) + transform->GetWPos();
	pVtx[1].pos = D3DXVECTOR3(SIZE * 0.5f, SIZE * -0.5f, 0.0f) + transform->GetWPos();
	pVtx[2].pos = D3DXVECTOR3(SIZE * -0.5f, SIZE * 0.5f, 0.0f) + transform->GetWPos();
	pVtx[3].pos = D3DXVECTOR3(SIZE * 0.5f, SIZE * 0.5f, 0.0f) + transform->GetWPos();

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
	m_terrainVtxBuff->Unlock();

	// ���s�f�[�^�Ɋ�Â��Ē��_�𐶐�����
	unsigned int numData = travellingData.size();
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * numData, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_travellingVtxBuff, nullptr);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_travellingVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (unsigned int i = 0; i < numData; i++)
	{
		// ���_���W�̐ݒ�
		D3DXVECTOR3 pos = travellingData[i].pos / (Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE) * SIZE;
		pos += transform->GetWPos();
		pos.z = 0.0f;
		pVtx[0].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * -0.5f, TRAVELLING_POINT_SIZE * -0.5f, 0.0f) + pos;
		pVtx[1].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * 0.5f, TRAVELLING_POINT_SIZE * -0.5f, 0.0f) + pos;
		pVtx[2].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * -0.5f, TRAVELLING_POINT_SIZE * 0.5f, 0.0f) + pos;
		pVtx[3].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * 0.5f, TRAVELLING_POINT_SIZE * 0.5f, 0.0f) + pos;

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

		// ���̃|�C���^��
		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_travellingVtxBuff->Unlock();
}

//=============================================================
// [ResultTerrain] �I��
//=============================================================
void ResultTerrain::Uninit()
{
	// �e�N�X�`���̔j��
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}

	// �n�`�̔j��
	if (m_terrainVtxBuff != nullptr)
	{
		m_terrainVtxBuff->Release();
		m_terrainVtxBuff = nullptr;
	}

	if (m_travellingVtxBuff != nullptr)
	{
		m_travellingVtxBuff->Release();
		m_travellingVtxBuff = nullptr;
	}
}

//=============================================================
// [ResultTerrain] �X�V
//=============================================================
void ResultTerrain::Update()
{
	auto travellingData = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTravellingData();
	if (travellingData.size() <= 0)
	{
		return;
	}

	m_travellingCounter--;
	if (m_travellingCounter <= 0)
	{ // ���s�J�E���g���B�����Ƃ�
		m_travellingIdx++;

		if (m_travellingIdx >= travellingData.size() - 1)
		{ // �ŏI�|�C���g�̂Ƃ�
			m_travellingCounter = TRAVELLING_ENDFRAME;
		}
		else
		{ // �ʏ�
			m_travellingCounter = TRAVELLING_FRAME;
		}

		if (m_travellingIdx >= travellingData.size())
		{ // �C���f�b�N�X���ő�l�ɒB�����Ƃ�
			m_travellingIdx = 0;
		}
	}
}

//=============================================================
// [ResultTerrain] �`��
//=============================================================
void ResultTerrain::DrawUI()
{
	auto travellingData = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTravellingData();
	if (travellingData.size() <= 0)
	{
		return;
	}

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetInstance()->GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_terrainVtxBuff, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, m_texture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0, //�`�悷��ŏ��̒��_�C���f�b�N�X
		2); //�`�悷��v���~�e�B�u��


	// ���s�f�[�^�̕`��
	for (int i = 0; i < travellingData.size(); i++)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_travellingVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, nullptr);

		// �|���S���̕`��
		if (i <= m_travellingIdx)
		{
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
				i * 4, //�`�悷��ŏ��̒��_�C���f�b�N�X
				2); //�`�悷��v���~�e�B�u��
		}
	}
}