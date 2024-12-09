//=============================================================
//
// �C���x���g�� [inventory.h]
// Author: Ito Kotaro
//
//=============================================================
#include "inventory.h"
#include "scripts/item/item_manager.h"
#include "manager.h"

//=============================================================
// [InventoryUI] ������
//=============================================================
void InventoryUI::Init()
{
	// �w�i
	m_bg = new SingleComponent<CPolygon>();
	m_bg->Init();
	m_bg->SetParent(gameObject);
	m_bg->transform->SetSize(900.0f, 200.0f);

	// �O��
	m_backArrow = new SingleComponent<ButtonUI>();
	m_backArrow->SetParent(gameObject);
	m_backArrow->Init();
	m_backArrow->SetTexture("data\\TEXTURE\\INVENTORY\\arrow.png");
	m_backArrow->SetReverse(true);
	m_backArrow->transform->SetPos(15.0f, 22.5f);
	m_backArrow->transform->SetSize(100.0f, 150.0f);
	m_backArrow->setClickEvent([this]() {m_headIdx--; });

	// ����
	m_nextArrow = new SingleComponent<ButtonUI>();
	m_nextArrow->SetParent(gameObject);
	m_nextArrow->Init();
	m_nextArrow->SetTexture("data\\TEXTURE\\INVENTORY\\arrow.png");
	m_nextArrow->transform->SetPos(780.0f, 22.5f);
	m_nextArrow->transform->SetSize(100.0f, 150.0f);
	m_nextArrow->setClickEvent([this]() {m_headIdx++; });

	// �A�C�e���t���[��
	for (int i = 0; i < 4; i++)
	{
		m_itemFrame[i] = new SingleComponent<CPolygon>();
		m_itemFrame[i]->SetParent(gameObject);
		m_itemFrame[i]->Init();
		m_itemFrame[i]->SetTexture("data\\TEXTURE\\INVENTORY\\frame.png");
		m_itemFrame[i]->transform->SetSize(160.0f, 160.0f);
		m_itemFrame[i]->transform->SetPos(170.0f * i + 112.5f, 20.0f);
		m_itemFrame[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		m_itemBG[i] = new SingleComponent<CPolygon>();
		m_itemBG[i]->SetParent(gameObject);
		m_itemBG[i]->Init();
		m_itemBG[i]->transform->SetSize(120.0f, 120.0f);
		m_itemBG[i]->transform->SetPos(170.0f * i + 132.5f, 40.0f);
		m_itemBG[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 50));

		m_itemTexture[i] = new SingleComponent<CPolygon>();
		m_itemTexture[i]->SetParent(gameObject);
		m_itemTexture[i]->Init();
		m_itemTexture[i]->SetTexture("data\\TEXTURE\\ITEM\\fuel_tank.png");
		m_itemTexture[i]->transform->SetSize(120.0f, 120.0f);
		m_itemTexture[i]->transform->SetPos(170.0f * i + 132.5f, 40.0f);
		m_itemTexture[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}
}

//=============================================================
// [InventoryUI] �I��
//=============================================================
void InventoryUI::Uninit()
{
	m_bg->Uninit();
	delete m_bg;

	m_backArrow->Uninit();
	delete m_backArrow;

	m_nextArrow->Uninit();
	delete m_nextArrow;

	for (int i = 0; i < 4; i++)
	{
		m_itemFrame[i]->Uninit();
		delete m_itemFrame[i];

		m_itemBG[i]->Uninit();
		delete m_itemBG[i];

		m_itemTexture[i]->Uninit();
		delete m_itemTexture[i];
	}
}

//=============================================================
// [InventoryUI] �X�V
//=============================================================
void InventoryUI::Update()
{
	auto pInventory = ItemManager::GetInstance()->GetInventory();

	// �C���x���g���̈ʒu��ύX����
	if (m_headIdx > static_cast<int>(pInventory->size()) - 1)
	{
		m_headIdx = static_cast<int>(pInventory->size()) - 1;
	}
	if (m_headIdx < 0)
	{
		m_headIdx = 0;
	}

	// �A�C�e���̕\������
	for (int i = 0; i < 4; i++)
	{
		// ������
		m_itemFrame[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		if (m_headIdx + i < static_cast<int>(pInventory->size()))
		{ // �w��̃X���b�g�ɃA�C�e��������Ƃ�
			m_itemTexture[i]->SetTexture(pInventory->at(m_headIdx + i)->GetTexturePath());

			// �I������Ă���Ƃ�
			if (pInventory->at(m_headIdx + i) == ItemManager::GetInstance()->GetCarryOn(0) ||
				pInventory->at(m_headIdx + i) == ItemManager::GetInstance()->GetCarryOn(1))
			{
				m_itemFrame[i]->SetColor(D3DCOLOR_RGBA(255, 67, 20, 255));
			}
		}
		else
		{ // �w��̃X���b�g�ɃA�C�e�����Ȃ��Ƃ�
			m_itemTexture[i]->SetTexture("");
		}
	}

	// �A�C�e���̑I���E��I������
	auto cPos = CManager::GetInstance()->GetCursorClientPos();
	for (int i = 0; i < 4; i++)
	{
		if (m_headIdx + i < static_cast<int>(pInventory->size()))
		{ // �w��̃X���b�g�ɃA�C�e��������Ƃ�
			auto wFramePos = m_itemFrame[i]->transform->GetWPos();

			if (wFramePos.x <= cPos.x && cPos.x <= wFramePos.x + 160.0f &&
				wFramePos.y <= cPos.y && cPos.y <= wFramePos.y + 160.0f &&
				INPUT_INSTANCE->onTrigger("lclick"))
			{ // �N���b�N���ꂽ�Ƃ�
				ItemManager::GetInstance()->ToggleCarryOn(pInventory->at(m_headIdx + i));
			}
		}
	}

	// �R���|�[�l���g�̍X�V
	m_bg->Update();
	m_backArrow->Update();
	m_nextArrow->Update();

	for (int i = 0; i < 4; i++)
	{
		m_itemFrame[i]->Update();
		m_itemBG[i]->Update();
		m_itemTexture[i]->Update();
	}
}

//=============================================================
// [InventoryUI] �`��
//=============================================================
void InventoryUI::DrawUI()
{
	//m_bg->DrawUI();
	m_backArrow->DrawUI();
	m_nextArrow->DrawUI();

	for (int i = 0; i < 4; i++)
	{
		m_itemBG[i]->DrawUI();
		m_itemTexture[i]->DrawUI();
		m_itemFrame[i]->DrawUI();
	}	
}