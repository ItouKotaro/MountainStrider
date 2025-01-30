//=============================================================
//
// �A�C�e���X���b�g [item_slot.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "item_slot.h"
#include "manager.h"
#include "item_manager.h"

//=============================================================
// [ItemSlot] ������
//=============================================================
void ItemSlot::Init()
{
	// �A�C�e���t���[��
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i] = new SingleComponent<CPolygon>();
		m_itemFrame[i]->SetParent(gameObject);
		m_itemFrame[i]->Init();
		m_itemFrame[i]->SetTexture("data\\TEXTURE\\INVENTORY\\frame.png");
		m_itemFrame[i]->transform->SetSize(160.0f, 160.0f);
		m_itemFrame[i]->transform->SetPos(170.0f * i, 0.0f);
		m_itemFrame[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));

		m_itemBG[i] = new SingleComponent<CPolygon>();
		m_itemBG[i]->SetParent(gameObject);
		m_itemBG[i]->Init();
		m_itemBG[i]->transform->SetSize(120.0f, 120.0f);
		m_itemBG[i]->transform->SetPos(170.0f * i + 20.0f, 20.0f);
		m_itemBG[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 50));

		m_itemTexture[i] = new SingleComponent<CPolygon>();
		m_itemTexture[i]->SetParent(gameObject);
		m_itemTexture[i]->Init();
		m_itemTexture[i]->transform->SetSize(120.0f, 120.0f);
		m_itemTexture[i]->transform->SetPos(170.0f * i + 20.0f, 20.0f);
		m_itemTexture[i]->SetColor(D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	// �A�C�e���̃e�N�X�`����ύX����
	for (int i = 0; i < 2; i++)
	{
		if (ItemManager::GetInstance()->GetCarryOn(i) != nullptr)
		{ // �A�C�e������
			m_itemTexture[i]->SetTexture(ItemManager::GetInstance()->GetCarryOn(i)->GetTexturePath());
			m_itemTexture[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		}
		else
		{ // �A�C�e���Ȃ�
			m_itemTexture[i]->SetTexture("");
			m_itemTexture[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f));
		}
	}

	// �؂�ւ��K�C�h���쐬����
	m_itemSlotToggleGuide = new SingleComponent<CPolygon>();
	m_itemSlotToggleGuide->Init();
	m_itemSlotToggleGuide->SetParent(gameObject);
	m_itemSlotToggleGuide->transform->SetPos(110.0f, 30.0f, 0.0f);
	m_itemSlotToggleGuide->transform->SetSize(100.0f, 100.0f);

	// ����f�o�C�X�ŕ\������K�C�h��ύX����
	m_itemSlotToggleGuide->SetTexture(
		INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER ?
		"data\\TEXTURE\\UI\\item_toggle_c.png" : "data\\TEXTURE\\UI\\item_toggle_m.png"
	);

	// �A�C�e���̎g�p���@�̃K�C�h��ݒ肷��
	m_itemUseGuide = new SingleComponent<CPolygon>();
	m_itemUseGuide->Init();
	m_itemUseGuide->SetParent(gameObject);
	m_itemUseGuide->transform->SetSize(50.0f, 50.0f);
	m_itemUseGuide->transform->SetPos(m_selectIdx == 0 ? 55.0f : 225.0f, 160.0f);

	m_itemUseGuide->SetTexture(
		INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER ?
		"data\\TEXTURE\\UI\\CONTROLLER\\button_y.png" : "data\\TEXTURE\\UI\\KEYBOARD\\key_f.png"
	);

	// �����쐬����
	m_seManager = new GameObject();
	m_seManager->AddComponent<AudioSource>();
	m_useSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\use.wav", FMOD_2D);
}

//=============================================================
// [ItemSlot] �I��
//=============================================================
void ItemSlot::Uninit()
{
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i]->Uninit();
		delete m_itemFrame[i];

		m_itemBG[i]->Uninit();
		delete m_itemBG[i];

		m_itemTexture[i]->Uninit();
		delete m_itemTexture[i];
	}

	m_itemSlotToggleGuide->Uninit();
	delete m_itemSlotToggleGuide;

	m_itemUseGuide->Uninit();
	delete m_itemUseGuide;

	m_seManager->Destroy();
}

//=============================================================
// [ItemSlot] �X�V
//=============================================================
void ItemSlot::Update()
{
	// �I���ʒu��ύX����
	float scroll = CManager::GetInstance()->GetMouseWheel();
	if (scroll > 10.0f)
	{
		m_selectIdx--;
	}
	else if (scroll < -10.0f)
	{
		m_selectIdx++;
	}
	if (INPUT_INSTANCE->onInput("toggle-item"))
	{
		if (m_selectIdx == 0) m_selectIdx = 1;
		else if (m_selectIdx == 1) m_selectIdx = 0;
	}

	// �I���̏������
	if (m_selectIdx < 0) m_selectIdx = 0;
	else if (m_selectIdx > 1) m_selectIdx = 1;

	// �I���̘g�̐F��ύX����
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i]->SetColor(i == m_selectIdx ? D3DCOLOR_RGBA(255, 67, 20, 255) : D3DCOLOR_RGBA(255, 255, 255, 255));
	}

	// �g�p�L�[�������ꂽ�Ƃ��A�A�C�e�����g�p����
	if (INPUT_INSTANCE->onInput("use-item"))
	{
		if (ItemManager::GetInstance()->GetCarryOn(m_selectIdx) != nullptr)
		{
			ItemManager::GetInstance()->GetCarryOn(m_selectIdx)->onUse();
			m_seManager->GetComponent<AudioSource>()->PlayOneShot(m_useSE);
		}

		// �e�N�X�`���̍X�V
		for (int i = 0; i < 2; i++)
		{
			if (ItemManager::GetInstance()->GetCarryOn(i) != nullptr)
			{ // �A�C�e������
				m_itemTexture[i]->SetTexture(ItemManager::GetInstance()->GetCarryOn(i)->GetTexturePath());
				m_itemTexture[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			}
			else
			{ // �A�C�e���Ȃ�
				m_itemTexture[i]->SetTexture("");
				m_itemTexture[i]->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.1f));
			}
		}
	}

	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i]->Update();
		m_itemBG[i]->Update();
		m_itemTexture[i]->Update();
	}

	m_itemSlotToggleGuide->Update();
	m_itemUseGuide->Update();

	// ����f�o�C�X�ŕ\������K�C�h��ύX����
	m_itemSlotToggleGuide->SetTexture(
		INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER ?
		"data\\TEXTURE\\UI\\item_toggle_c.png" : "data\\TEXTURE\\UI\\item_toggle_m.png"
	);

	m_itemUseGuide->SetTexture(
		INPUT_INSTANCE->GetLastInput() == INPUT_INSTANCE->DEVICE_CONTROLLER ?
		"data\\TEXTURE\\UI\\CONTROLLER\\button_y.png" : "data\\TEXTURE\\UI\\KEYBOARD\\key_f.png"
	);
	m_itemUseGuide->transform->SetPos(m_selectIdx == 0 ? 55.0f : 225.0f, 160.0f);
}

//=============================================================
// [ItemSlot] �`��
//=============================================================
void ItemSlot::DrawUI()
{
	for (int i = 0; i < 2; i++)
	{
		m_itemFrame[i]->DrawUI();
		m_itemBG[i]->DrawUI();
		m_itemTexture[i]->DrawUI();
	}

	m_itemSlotToggleGuide->DrawUI();
	m_itemUseGuide->DrawUI();
}