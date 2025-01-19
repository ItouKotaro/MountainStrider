//=============================================================
//
// �� [lake.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "lake.h"
#include "renderer.h"
#include "internal/data_manager.h"

//=============================================================
// [LakeManager] ������
//=============================================================
void LakeManager::Init(Terrain* terrain, const std::string& path)
{
	// �o�C�N���擾����
	m_vehicle = GameObject::Find("Vehicle")->GetComponent<CVehicle>();

	// json�t�@�C����ǂݍ���
	std::ifstream ifs(path.c_str());

	if (ifs.fail())
	{ // �t�@�C���̓ǂݍ��݂Ɏ��s
		return;
	}

	// json�`���ɕϊ�
	std::string sInputData((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
	auto jInput = json::parse(sInputData);

	if (jInput.contains("terrain") && jInput["terrain"].contains("lake"))
	{
		m_enabled = true;

		// �΂̍�����ݒ肷��
		if (jInput["terrain"]["lake"].contains("height"))
		{
			float height = jInput["terrain"]["lake"]["height"];
			float rate = (height + 1.0f) / static_cast<float>(2.0f);
			m_height = terrain->GetMinHeight() + (terrain->GetMaxHeight() - terrain->GetMinHeight()) * rate;
		}

		// �R���_���[�W��ݒ肷��
		if (jInput["terrain"]["lake"].contains("fuel_damage"))
		{
			m_fuelDamage = jInput["terrain"]["lake"]["fuel_damage"];
		}

		// �ϋv�l�_���[�W��ݒ肷��
		if (jInput["terrain"]["lake"].contains("endurance_damage"))
		{
			m_enduranceDamage = jInput["terrain"]["lake"]["endurance_damage"];
		}

		// �e�N�X�`����ݒ肷��
		if (jInput["terrain"]["lake"].contains("textures"))
		{
			for (auto itr = jInput["terrain"]["lake"]["textures"].begin(); itr != jInput["terrain"]["lake"]["textures"].end(); itr++)
			{
				GameObject* lakeFieldObj = new GameObject();
				lakeFieldObj->transform->SetPos(0.0f, m_height, 0.0f);
				auto lakeField = lakeFieldObj->AddComponent<LakeField>();

				// �T�C�Y��ݒ肷��
				lakeField->SetSize(Terrain::TERRAIN_DISTANCE, Terrain::TERRAIN_DISTANCE);

				std::string lakePath = "";
				int lakeLoop = 1;
				D3DXVECTOR2 lakeMove = { 0.0f, 0.0f };

				// �e�N�X�`���p�X�̎擾
				if (itr.value().contains("path"))
				{
					lakePath = itr.value()["path"];
				}

				// ���[�v���̎擾
				if (itr.value().contains("loop"))
				{
					lakeLoop = itr.value()["loop"];
				}

				// �ړ��ʂ̎擾
				if (itr.value().contains("move"))
				{
					lakeMove.x = itr.value()["move"][0];
					lakeMove.y = itr.value()["move"][1];
				}

				// �e�N�X�`����ݒ肷��
				lakeField->SetTexture(lakePath, lakeLoop, lakeMove);

				// �F��ݒ肷��
				if (itr.value().contains("color"))
				{
					lakeField->SetColor(D3DCOLOR_RGBA(itr.value()["color"][0], itr.value()["color"][1], itr.value()["color"][2], itr.value()["color"][3]));
				}
			}
		}
	}
}

//=============================================================
// [LakeManager] �X�V
//=============================================================
void LakeManager::Update()
{
	D3DXVECTOR3 pos = m_vehicle->transform->GetWPos();
	if (m_enabled && pos.y <= m_height &&
		-Terrain::TERRAIN_DISTANCE_HALF <= pos.x && pos.x <= Terrain::TERRAIN_DISTANCE_HALF &&
		-Terrain::TERRAIN_DISTANCE_HALF <= pos.z && pos.z <= Terrain::TERRAIN_DISTANCE_HALF)
	{
		// �_���[�W��^����
		m_vehicle->AddDamage(m_enduranceDamage);
		m_vehicle->AddFuel(-m_fuelDamage);
	}
}



//=============================================================
// [LakeField] ������
//=============================================================
void LakeField::Init()
{
	// �ϐ��̏�����
	m_pVtxBuff = nullptr;											// ���_�o�b�t�@
	m_pTexture = nullptr;										// �e�N�X�`��
	m_color = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);		// �J���[
	m_move = { 0.0f, 0.0f };
	m_texMove = { 0.0f, 0.0f };

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();

	//���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_3D* pVtx; //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

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
	m_pVtxBuff->Unlock();
}

//=============================================================
// [LakeField] �I��
//=============================================================
void LakeField::Uninit()
{
	// ���_�o�b�t�@�̔j��
	if (m_pVtxBuff != nullptr)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = nullptr;
	}
}

//=============================================================
// [LakeField] �X�V
//=============================================================
void LakeField::Update()
{
	// �e�N�X�`���̈ړ��l�����Z����
	m_texMove += m_move;

	// ����������
	if (m_texMove.x <= -1.0f)
		m_texMove.x += 1.0f;
	else if (m_texMove.x >= 1.0f)
		m_texMove.x -= 1.0f;
	if (m_texMove.y <= -1.0f)
		m_texMove.y += 1.0f;
	else if (m_texMove.y >= 1.0f)
		m_texMove.y -= 1.0f;

	// �e�N�X�`�����X�V����
	VERTEX_3D* pVtx; //���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�̐ݒ�
	float numLoop = static_cast<float>(m_loopNum);
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f) + m_texMove;
	pVtx[1].tex = D3DXVECTOR2(numLoop, 0.0f) + m_texMove;
	pVtx[2].tex = D3DXVECTOR2(0.0f, numLoop) + m_texMove;
	pVtx[3].tex = D3DXVECTOR2(numLoop, numLoop) + m_texMove;

	// ���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================
// [LakeField] �`��
//=============================================================
void LakeField::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CRenderer::GetInstance()->GetDevice();		// �f�o�C�X���擾
	D3DXMATRIX mtx = transform->GetMatrix();

	Component::BeginPass();

	// ���C�g�𖳌��ɂ���
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O���I�t�ɂ���
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// ���[���h�}�g���b�N�X�̐ݒ�
	if (!IsEnabledShader()) pDevice->SetTransform(D3DTS_WORLD, &mtx);

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	if (!IsEnabledShader()) pDevice->SetTexture(0, m_pTexture);

	Shader::ParamData paramData;
	paramData.color = m_color;
	paramData.texture = m_pTexture;
	paramData.mtx = transform->GetMatrix();
	Component::SetParam(paramData);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
		0, //�`�悷��ŏ��̒��_�C���f�b�N�X
		2);				//�`�悷��v���~�e�B�u��

	Component::EndPass();

	// ���C�g��L���ɖ߂�
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �J�����O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================
// [LakeField] �T�C�Y�̐ݒ�
//=============================================================
void LakeField::SetSize(const float& x, const float& y)
{
	VERTEX_3D* pVtx; //���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(-x * 0.5f, 0.0f, y * 0.5f);
	pVtx[1].pos = D3DXVECTOR3(x * 0.5f, 0.0f, y * 0.5f);
	pVtx[2].pos = D3DXVECTOR3(-x * 0.5f, 0.0f, -y * 0.5f);
	pVtx[3].pos = D3DXVECTOR3(x * 0.5f, 0.0f, -y * 0.5f);

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}

//=============================================================
// [LakeField] �e�N�X�`����ݒ肷��
//=============================================================
void LakeField::SetTexture(const std::string& texturePath, const int& loopNum, const D3DXVECTOR2& move)
{
	// �e�N�X�`����ݒ肷��
	BindTexture(CDataManager::GetInstance()->RefTexture(texturePath)->GetTexture());

	// ���[�v�񐔂�ݒ肷��
	m_loopNum = loopNum;

	// �ړ��l��ݒ肷��
	m_move = move;
}

//=============================================================
// [LakeField] �F��ݒ肷��
//=============================================================
void LakeField::SetColor(const D3DXCOLOR& color)
{
	m_color = color;

	VERTEX_3D* pVtx; //���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[
	pVtx[0].col = m_color	;
	pVtx[1].col = m_color;
	pVtx[2].col = m_color;
	pVtx[3].col = m_color;

	//���_�o�b�t�@���A�����b�N����
	m_pVtxBuff->Unlock();
}