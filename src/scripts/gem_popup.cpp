//=============================================================
//
// �W�F���̃|�b�v�A�b�v�\��UI [gem_popup.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "gem_popup.h"
#include "manager.h"
#include "shop/shop.h"

const D3DXVECTOR2 GemPopupUI::BG_SIZE = { 500.0f, 150.0f };

//=============================================================
// [GemPopupUI] ������
//=============================================================
void GemPopupUI::Init()
{
	// �ϐ��̏�����
	m_oldPoint = 0;
	m_showTime = 0.0f;

	// �w�i
	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetTexture("data\\TEXTURE\\UI\\gem_popup.png");
	m_bg->SetParent(gameObject);
	m_bg->transform->SetSize(BG_SIZE);

	// �e�L�X�g
	m_text = new SingleComponent<CText>();
	m_text->Init();
	m_text->SetParent(gameObject);
	m_text->SetText("0");
	m_text->SetFontSize(120);
	m_text->SetFontColor(D3DCOLOR_RGBA(41, 123, 230, 255));
	m_text->SetOutlineColor(D3DCOLOR_RGBA(41, 123, 230, 255));
	m_text->SetFont("07�S�r�S�V�b�N");
	m_text->SetAlign(CText::CENTER);
	m_text->transform->SetPos(300.0f, 35.0f);
}

//=============================================================
// [GemPopupUI] �I��
//=============================================================
void GemPopupUI::Uninit()
{
	if (m_bg != nullptr)
	{
		m_bg->Uninit();
		delete m_bg;
		m_bg = nullptr;
	}

	if (m_text != nullptr)
	{
		m_text->Uninit();
		delete m_text;
		m_text = nullptr;
	}
}

//=============================================================
// [GemPopupUI] �X�V
//=============================================================
void GemPopupUI::Update()
{
	// �|�C���g�ɕϓ����������Ƃ�
	if (ShopManager::GetPoints() != m_oldPoint)
	{
		// �\�����Ԃ�ݒ肷��
		m_showTime = SHOW_TIME;

		// �e�L�X�g���X�V����
		m_text->SetText(std::to_string(ShopManager::GetPoints()));

		// ���l���X�V����
		m_oldPoint = ShopManager::GetPoints();
	}

	// �\��
	m_showTime -= CManager::GetInstance()->GetDeltaTime();
	if (m_showTime > 0.0f)
	{ // �\����
		transform->SetPos((float)CRenderer::SCREEN_WIDTH / 2.0f - GemPopupUI::BG_SIZE.x / 2.0f, transform->GetPosY() + (10.0f - transform->GetPosY()) * DAMPING);
	}
	else
	{ // ��\����
		transform->SetPos((float)CRenderer::SCREEN_WIDTH / 2.0f - GemPopupUI::BG_SIZE.x / 2.0f, transform->GetPosY() + (-200.0f - transform->GetPosY()) * DAMPING);
	}

	m_bg->Update();
	m_text->Update();
}

//=============================================================
// [GemPopupUI] UI�`��
//=============================================================
void GemPopupUI::DrawUI()
{
	m_bg->DrawUI();
	m_text->DrawUI();
}
