//=============================================================
//
// �w���{�^�� [buy_button.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "buy_button.h"
#include "renderer.h"
#include "manager.h"
#include "shop.h"

const float BuyButtonUI::EDGE_BOLD = 15.0f;
const float BuyButtonUI::WIDTH = 280.0f;
const float BuyButtonUI::HEIGHT = 400.0f;
const float BuyButtonUI::ITEM_SIZE = 146.0f;
const float BuyButtonUI::PRICEICON_SIZE = 60.0f;
const D3DXCOLOR BuyButtonUI::ITEM_COLOR = D3DCOLOR_RGBA(34, 130, 43, 255);
const D3DXCOLOR BuyButtonUI::ITEM_COLOR_BOUGHT = D3DCOLOR_RGBA(2, 54, 7, 255);
const D3DXCOLOR BuyButtonUI::PERK_COLOR = D3DCOLOR_RGBA(196, 0, 194, 255);
const D3DXCOLOR BuyButtonUI::PERK_COLOR_BOUGHT = D3DCOLOR_RGBA(107, 22, 106, 255);
//=============================================================
// [BuyButtonUI] ������
//=============================================================
void BuyButtonUI::Init()
{
	m_progress = 0.0f;

	// �C���[�W�摜
	m_image = new SingleComponent<CPolygon>();
	m_image->Init();
	m_image->SetTexture(m_shopItem->GetTexturePath());
	m_image->SetParent(gameObject);
	m_image->transform->SetSize(ITEM_SIZE, ITEM_SIZE);
	m_image->transform->SetPos(WIDTH / 2.0f - ITEM_SIZE / 2.0f, 30.0f, 0.0f);

	// ���i�A�C�R��
	m_priceIcon = new SingleComponent<CPolygon>();
	m_priceIcon->Init();
	m_priceIcon->SetTexture("data\\TEXTURE\\SHOP\\point.png");
	m_priceIcon->SetParent(gameObject);
	m_priceIcon->transform->SetSize(PRICEICON_SIZE, PRICEICON_SIZE);
	m_priceIcon->transform->SetPos(50.0f, 325.0f);

	// �A�C�e����
	m_itemName = new SingleComponent<CText>();
	m_itemName->Init();
	m_itemName->SetParent(gameObject);
	m_itemName->transform->SetPos(WIDTH / 2.0f, 200.0f);
	m_itemName->SetFontSize(50);
	m_itemName->SetAlign(CText::CENTER);
	m_itemName->SetFont("07�S�r�S�V�b�N");
	m_itemName->SetText(m_shopItem->GetName());

	// ����
	m_description = new SingleComponent<CText>();
	m_description->Init();
	m_description->SetParent(gameObject);
	m_description->transform->SetPos(WIDTH / 2.0f, 260.0f);
	m_description->SetFontSize(26);
	m_description->SetAlign(CText::CENTER);
	m_description->SetFontColor(D3DCOLOR_RGBA(224, 224, 224, 255));
	m_description->SetOutlineColor(D3DCOLOR_RGBA(224, 224, 224, 255));
	m_description->SetFont("07�S�r�S�V�b�N");
	m_description->SetText(m_shopItem->GetDescription());

	// ���i�e�L�X�g
	m_priceText = new SingleComponent<CText>();
	m_priceText->Init();
	m_priceText->SetParent(gameObject);
	m_priceText->transform->SetPos(WIDTH / 2.0f + 30.0f, 335.0f);
	m_priceText->SetFontSize(50);
	m_priceText->SetAlign(CText::CENTER);
	m_priceText->SetFont("07�S�r�S�V�b�N");
	m_priceText->SetText(std::to_string(m_shopItem->GetPrice()));

	// �d������
	m_splitLine = new SingleComponent<CPolygon>();
	m_splitLine->Init();
	m_splitLine->SetParent(gameObject);
	m_splitLine->transform->SetSize(WIDTH, 5.0f);
	m_splitLine->transform->SetPos(0.0f, 310.0f);

	// ��
	m_edge = new SingleComponent<CPolygon>();
	m_edge->Init();
	m_edge->SetTexture("data\\TEXTURE\\SHOP\\buy_button_mask.png");
	m_edge->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	m_edge->SetParent(gameObject);
	m_edge->transform->SetSize(WIDTH, HEIGHT);

	// �w����
	m_bought = new SingleComponent<CPolygon>();
	m_bought->Init();
	m_bought->SetTexture("data\\TEXTURE\\SHOP\\buy_button_mask.png");
	m_bought->SetParent(gameObject);
	m_bought->transform->SetSize(WIDTH - EDGE_BOLD, HEIGHT - EDGE_BOLD);
	m_bought->transform->SetPos(EDGE_BOLD/2.0f, EDGE_BOLD / 2.0f);

	// �w���O
	m_inside = new SingleComponent<CPolygon>();
	m_inside->Init();
	m_inside->SetTexture("data\\TEXTURE\\SHOP\\buy_button_mask.png");
	m_inside->SetParent(gameObject);
	m_inside->transform->SetSize(WIDTH - EDGE_BOLD, HEIGHT - EDGE_BOLD);
	m_inside->transform->SetPos(EDGE_BOLD / 2.0f, EDGE_BOLD / 2.0f);

	switch (m_shopItem->GetType())
	{
	case ShopItem::TYPE::ITEM:
		m_bought->SetColor(ITEM_COLOR_BOUGHT);
		m_inside->SetColor(ITEM_COLOR);
		break;
	case ShopItem::TYPE::PERK:
		m_bought->SetColor(PERK_COLOR_BOUGHT);
		m_inside->SetColor(PERK_COLOR);
		break;
	}

	// �}�X�N
	m_mask = new SingleComponent<CPolygon>();
	m_mask->Init();
	m_mask->SetParent(gameObject);
	m_mask->transform->SetSize(WIDTH, HEIGHT);
}

//=============================================================
// [BuyButtonUI] �I��
//=============================================================
void BuyButtonUI::Uninit()
{
	m_image->Uninit();
	delete m_image;

	m_priceIcon->Uninit();
	delete m_priceIcon;

	m_itemName->Uninit();
	delete m_itemName;

	m_description->Uninit();
	delete m_description;

	m_priceText->Uninit();
	delete m_priceText;

	m_splitLine->Uninit();
	delete m_splitLine;

	m_edge->Uninit();
	delete m_edge;

	m_bought->Uninit();
	delete m_bought;

	m_inside->Uninit();
	delete m_inside;

	m_mask->Uninit();
	delete m_mask;
}

//=============================================================
// [BuyButtonUI] �X�V
//=============================================================
void BuyButtonUI::Update()
{
	// �J�[�\���̈ʒu���擾����
	auto cursorPos = CManager::GetInstance()->GetCursorClientPos();

	// �N���b�N����Ă��邩�𔻕ʂ���
	D3DXVECTOR3 wPos = transform->GetWPos();
	bool isClicked = false;
	if (wPos.x <= cursorPos.x && cursorPos.x <= wPos.x + WIDTH &&
		wPos.y <= cursorPos.y && cursorPos.y <= wPos.y + HEIGHT)
	{ // �{�^���͈͓̔�
		if (INPUT_INSTANCE->onPress("lclick") && 
			m_shopItem->GetShopManager()->GetPoints() >= m_shopItem->GetPrice())
		{ // ���N���b�N���|�C���g������Ă���Ƃ�
			isClicked = true;
		}
	}

	// �w���i�s�x
	m_progress += isClicked ? 0.03f : -0.05f;
	if (m_progress > 1.0f)
	{ // �w���m��
		m_progress = 0.0f;
		m_shopItem->onBuy();
	}
	else if (m_progress < 0.0f)
	{
		m_progress = 0.0f;
	}

	// �i�s�x�ɉ����ă|���S���𓮂���
	m_mask->transform->SetPos(0.0f, -HEIGHT * m_progress, 0.0f);


	// �|���S���̍X�V
	m_image->Update();
	m_priceIcon->Update();
	m_itemName->Update();
	m_description->Update();
	m_priceText->Update();
	m_splitLine->Update();
	m_edge->Update();
	m_bought->Update();
	m_inside->Update();
	m_mask->Update();
}

//=============================================================
// [BuyButtonUI] �`��
//=============================================================
void BuyButtonUI::DrawUI()
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

	m_edge->DrawUI();
	m_bought->DrawUI();

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
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_mask->DrawUI();

	pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pDevice->SetRenderState(D3DRS_STENCILENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
	pDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
	pDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x02);

	m_inside->DrawUI();

	// ���Ƃɖ߂�
	pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ZENABLE, dwCurZTest);
	pDevice->SetRenderState(D3DRS_ZFUNC, dwCurZFunc);

	pDevice->SetRenderState(D3DRS_STENCILREF, 0x01);
	pDevice->SetRenderState(D3DRS_STENCILMASK, 0xff);

	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x01);

	// �ʏ�`��
	m_image->DrawUI();
	m_itemName->DrawUI();
	m_description->DrawUI();
	m_priceIcon->DrawUI();
	m_priceText->DrawUI();
	m_splitLine->DrawUI();
}