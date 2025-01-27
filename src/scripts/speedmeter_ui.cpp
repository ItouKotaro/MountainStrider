//=============================================================
//
// �X�s�[�h���[�^�[UI [speedmeter_ui.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "speedmeter_ui.h"
#include "renderer.h"

//=============================================================
// [SpeedMeterUI] ������
//=============================================================
void SpeedMeterUI::Init()
{
	// �X�s�[�h�e�L�X�g�̏�����
	m_speedText = new SingleComponent<CText>();
	m_speedText->Init();
	m_speedText->SetParent(gameObject);
	m_speedText->SetFont("07�S�r�S�V�b�N");

	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 device = CRenderer::GetInstance()->GetDevice();

	// ���_�o�b�t�@�̐���
	device->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * BAR_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_vtxBuff, nullptr);
	VERTEX_2D* vtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	m_vtxBuff->Lock(0, 0, (void**)&vtx, 0);

	for (int i = 0; i < BAR_NUM; i++)
	{
		// �o�[�̍������Z�o����
		float height = i * i * 10.0f + 1.0f;

		// ���_���W�̐ݒ�
		vtx[0].pos = D3DXVECTOR3(0.0f, height, 0.0f);
		vtx[1].pos = D3DXVECTOR3(BAR_WIDTH, height, 0.0f);
		vtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		vtx[3].pos = D3DXVECTOR3(BAR_WIDTH, 0.0f, 0.0f);

		// �X�y�[�X
		vtx[0].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;
		vtx[1].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;
		vtx[2].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;
		vtx[3].pos.x += (BAR_WIDTH + BAR_SPACE) * i + BAR_RIGHT;

		vtx[0].pos.y += BAR_TOP;
		vtx[1].pos.y += BAR_TOP;
		vtx[2].pos.y += BAR_TOP;
		vtx[3].pos.y += BAR_TOP;

		// �g�����X�t�H�[���̊�Â��ʒu�Ɉړ�����
		vtx[0].pos += transform->GetWPos();
		vtx[1].pos += transform->GetWPos();
		vtx[2].pos += transform->GetWPos();
		vtx[3].pos += transform->GetWPos();

		// rhw�̐ݒ�
		vtx[0].rhw = 1.0f;
		vtx[1].rhw = 1.0f;
		vtx[2].rhw = 1.0f;
		vtx[3].rhw = 1.0f;

		// ���_�J���[
		vtx[0].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		vtx[1].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		vtx[2].col = D3DCOLOR_RGBA(255, 0, 0, 255);
		vtx[3].col = D3DCOLOR_RGBA(255, 0, 0, 255);

		// �e�N�X�`�����W�̐ݒ�
		vtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		vtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		vtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		vtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		// ���̃|�C���^�[��
		vtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	m_vtxBuff->Unlock();
}

//=============================================================
// [SpeedMeterUI] �I��
//=============================================================
void SpeedMeterUI::Uninit()
{
	m_speedText->Uninit();
	delete m_speedText;

	// ���_�o�b�t�@�̔j��
	if (m_vtxBuff != nullptr)
	{
		m_vtxBuff->Release();
		m_vtxBuff = nullptr;
	}
}

//=============================================================
// [SpeedMeterUI] �X�V
//=============================================================
void SpeedMeterUI::Update()
{
	// �X�s�[�h�e�L�X�g���X�V����
	char cSpeedText[12];
	sprintf(&cSpeedText[0], "%.1fKm/h", m_vehicle->GetSpeed());
	m_speedText->SetText(cSpeedText);


	// �X�V
	m_speedText->Update();
}

//=============================================================
// [SpeedMeterUI] UI�`��
//=============================================================
void SpeedMeterUI::DrawUI()
{
	// �`��
	m_speedText->DrawUI();

	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CRenderer::GetInstance()->GetDevice();

	for (int i = 0; i < BAR_NUM; i++)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, m_vtxBuff, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, nullptr);

		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, //�v���~�e�B�u�̎��
			i * 4, //�`�悷��ŏ��̒��_�C���f�b�N�X
			2); //�`�悷��v���~�e�B�u��
	}
}