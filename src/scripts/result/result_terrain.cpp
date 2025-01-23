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
#include "component/2d/text.h"

const std::string ResultTerrain::TERRAIN_TEX = "data\\convenience.bmp";
const int ResultTerrain::SIZE = 500;
const float ResultTerrain::TRAVELLING_POINT_SIZE = 5.0f;
const int ResultTerrain::TRAVELLING_FRAME = 1;
const int ResultTerrain::TRAVELLING_ENDFRAME = 120;
const float ResultTerrain::POINT_SIZE = SIZE / (float)Terrain::TERRAIN_SIZE;
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
	pVtx[0].pos = D3DXVECTOR3(SIZE * -0.5f, SIZE * -0.5f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SIZE * 0.5f, SIZE * -0.5f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(SIZE * -0.5f, SIZE * 0.5f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SIZE * 0.5f, SIZE * 0.5f, 0.0f);

	// �T�C�Y�Ɖ�]
	D3DXMATRIX rollMtx;
	D3DXMatrixIdentity(&rollMtx);
	D3DXMatrixRotationZ(&rollMtx, MAP_ROLL);
	D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &rollMtx);
	D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &rollMtx);
	D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &rollMtx);
	D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &rollMtx);

	pVtx[0].pos += transform->GetWPos();
	pVtx[1].pos += transform->GetWPos();
	pVtx[2].pos += transform->GetWPos();
	pVtx[3].pos += transform->GetWPos();

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
	unsigned int numData = static_cast<unsigned int>(travellingData.size());
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * numData, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_travellingVtxBuff, nullptr);

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_travellingVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (unsigned int i = 0; i < numData; i++)
	{
		// ���_���W�̐ݒ�
		D3DXVECTOR3 pos = (travellingData[i].pos / static_cast<float>((Terrain::TERRAIN_SIZE * Terrain::TERRAIN_SCALE))) * SIZE;
		pos.y = pos.z;
		pos.z = 0.0f;
		pVtx[0].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * -0.5f, TRAVELLING_POINT_SIZE * -0.5f, 0.0f) + pos;
		pVtx[1].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * 0.5f, TRAVELLING_POINT_SIZE * -0.5f, 0.0f) + pos;
		pVtx[2].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * -0.5f, TRAVELLING_POINT_SIZE * 0.5f, 0.0f) + pos;
		pVtx[3].pos = D3DXVECTOR3(TRAVELLING_POINT_SIZE * 0.5f, TRAVELLING_POINT_SIZE * 0.5f, 0.0f) + pos;

		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

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

	// �r�}�b�v�̏�����
	InitLakeMap();
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

	// ���s�}�b�v�̔j��
	if (m_travellingVtxBuff != nullptr)
	{
		m_travellingVtxBuff->Release();
		m_travellingVtxBuff = nullptr;
	}

	// �r�}�b�v�̔j��
	if (m_lakeVtxBuff != nullptr)
	{
		m_lakeVtxBuff->Release();
		m_lakeVtxBuff = nullptr;
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


	// �r�f�[�^�̕`��
	for (unsigned int i = 0; i < m_numLake; i++)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_lakeVtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, nullptr);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
			i * 4, //�`�悷��ŏ��̒��_�C���f�b�N�X
			2); //�`�悷��v���~�e�B�u��
	}


	// ���s�f�[�^�̕`��
	for (unsigned int i = 0; i < travellingData.size(); i++)
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

//=============================================================
// [ResultTerrain] �r�}�b�v�̏�����
//=============================================================
void ResultTerrain::InitLakeMap()
{
	struct LakePos
	{
		int x, y;
	};

	// �r�̈ʒu���擾����
	auto terrain = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetTerrain();
	auto lake = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene)->GetLake();
	std::vector<LakePos> lakePosList;
	m_numLake = 0;

	// �r���Ȃ��Ƃ��͏I������
	if (!lake->IsEnabled())
		return;

	for (int x = 0; x < Terrain::TERRAIN_SIZE; x++)
	{
		for (int y = 0; y < Terrain::TERRAIN_SIZE; y++)
		{
			float height = lake->GetHeight();
			float current = terrain->GetVertexHeight(x, y);
			if (current < height)
			{
				LakePos pos;
				pos.x = (Terrain::TERRAIN_SIZE-1) -x;
				pos.y = y;
				lakePosList.push_back(pos);
				m_numLake++;
			}
		}
	}

	if (m_numLake <= 0)
		return;


	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * m_numLake, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_lakeVtxBuff, nullptr);
	VERTEX_2D* pVtx; //���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_lakeVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (auto itr = lakePosList.begin(); itr != lakePosList.end(); itr++)
	{
		D3DXVECTOR3 putPos;
		putPos.x = (*itr).x * POINT_SIZE - SIZE *0.5f;
		putPos.y = (*itr).y * POINT_SIZE - SIZE * 0.5f;
		putPos.z = 0.0f;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f) + putPos ;
		pVtx[1].pos = D3DXVECTOR3(POINT_SIZE, 0.0f, 0.0f) + putPos;
		pVtx[2].pos = D3DXVECTOR3(0.0f, POINT_SIZE, 0.0f) + putPos;
		pVtx[3].pos = D3DXVECTOR3(POINT_SIZE, POINT_SIZE, 0.0f) + putPos;

		// �T�C�Y�Ɖ�]
		D3DXMATRIX rollMtx;
		D3DXMatrixIdentity(&rollMtx);
		D3DXMatrixRotationZ(&rollMtx, LAKE_ROLL);
		D3DXVec3TransformCoord(&pVtx[0].pos, &pVtx[0].pos, &rollMtx);
		D3DXVec3TransformCoord(&pVtx[1].pos, &pVtx[1].pos, &rollMtx);
		D3DXVec3TransformCoord(&pVtx[2].pos, &pVtx[2].pos, &rollMtx);
		D3DXVec3TransformCoord(&pVtx[3].pos, &pVtx[3].pos, &rollMtx);

		pVtx[0].pos += transform->GetWPos();
		pVtx[1].pos += transform->GetWPos();
		pVtx[2].pos += transform->GetWPos();
		pVtx[3].pos += transform->GetWPos();

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[
		pVtx[0].col = lake->GetBaseColor();
		pVtx[1].col = lake->GetBaseColor();
		pVtx[2].col = lake->GetBaseColor();
		pVtx[3].col = lake->GetBaseColor();

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_lakeVtxBuff->Unlock();
}
