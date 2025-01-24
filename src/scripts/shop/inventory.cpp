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

	// ���p�l��
	m_infoEdge = new SingleComponent<CPolygon>();
	m_infoEdge->Init();
	m_infoEdge->transform->SetSize(510.0f, 190.0f);
	m_infoEdge->transform->SetPos(-5.0f, -5.0f);
	m_infoEdge->SetColor(D3DCOLOR_RGBA(204, 204, 204, 255));

	m_infoBG = new SingleComponent<CPolygon>();
	m_infoBG->Init();
	m_infoBG->transform->SetSize(500.0f, 180.0f);
	m_infoBG->SetColor(D3DCOLOR_RGBA(50, 50, 50, 255));

	m_infoName = new SingleComponent<CText>();
	m_infoName->Init();
	m_infoName->SetFont("07�S�r�S�V�b�N");
	m_infoName->SetFontSize(55);
	m_infoName->SetParent(m_infoBG->transform);
	m_infoName->SetAlign(CText::CENTER);
	m_infoName->transform->SetPos(250.0f, 20.0f);

	m_infoDescription = new SingleComponent<CText>();
	m_infoDescription->Init();
	m_infoDescription->SetFont("07�S�r�S�V�b�N");
	m_infoDescription->SetFontSize(30);
	m_infoDescription->SetParent(m_infoBG->transform);
	m_infoDescription->SetAlign(CText::CENTER);
	m_infoDescription->transform->SetPos(250.0f, 95.0f);

	// ���̐���
	m_clickSE = AudioManager::GetInstance()->CreateClip("data\\SOUND\\SE\\click.mp3", FMOD_2D);
	m_sePlayer = new SingleComponent<AudioSource>();
	m_sePlayer->Init();
}

//=============================================================
// [InventoryUI] �I��
//=============================================================
void InventoryUI::Uninit()
{
	m_backArrow->Uninit();
	delete m_backArrow;

	m_nextArrow->Uninit();
	delete m_nextArrow;

	m_infoEdge->Uninit();
	delete m_infoEdge;

	m_infoBG->Uninit();
	delete m_infoBG;

	m_infoName->Uninit();
	delete m_infoName;

	m_infoDescription->Uninit();
	delete m_infoDescription;

	for (int i = 0; i < 4; i++)
	{
		m_itemFrame[i]->Uninit();
		delete m_itemFrame[i];

		m_itemBG[i]->Uninit();
		delete m_itemBG[i];

		m_itemTexture[i]->Uninit();
		delete m_itemTexture[i];
	}

	m_sePlayer->Uninit();
	delete m_sePlayer;

	AudioManager::GetInstance()->RemoveClip(m_clickSE);
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
				wFramePos.y <= cPos.y && cPos.y <= wFramePos.y + 160.0f)
			{
				// �A�C�e���ڍׂɃf�[�^����
				m_infoItem = pInventory->at(m_headIdx + i);

				// �N���b�N���ꂽ�Ƃ�
				if (INPUT_INSTANCE->onTrigger("lclick"))
				{
					// �؂�ւ���
					if (ItemManager::GetInstance()->ToggleCarryOn(pInventory->at(m_headIdx + i)))
					{
						// ����炷
						m_sePlayer->PlayOneShot(m_clickSE);
					}
				}
			}
		}
	}

	// �A�C�e���ڍ׃p�l�����X�V����
	if (m_infoItem != nullptr)
	{ // �A�C�e���̏�ɃJ�[�\��������Ƃ�
		// �ʒu��ύX����
		m_infoEdge->transform->SetPos(cPos.x - 5.0f, cPos.y - 5.0f - 180.0f);
		m_infoBG->transform->SetPos(cPos.x, cPos.y - 180.0f);

		// �����X�V����
		m_infoName->SetText(m_infoItem->GetName());
		m_infoDescription->SetText(m_infoItem->GetDescription());
	}

	// �R���|�[�l���g�̍X�V
	m_backArrow->Update();
	m_nextArrow->Update();
	m_infoEdge->Update();
	m_infoBG->Update();
	m_infoName->Update();
	m_infoDescription->Update();
	m_sePlayer->Update();

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
	m_backArrow->DrawUI();
	m_nextArrow->DrawUI();

	for (int i = 0; i < 4; i++)
	{
		m_itemBG[i]->DrawUI();
		m_itemTexture[i]->DrawUI();
		m_itemFrame[i]->DrawUI();
	}	

	if (m_infoItem != nullptr)
	{
		m_infoEdge->DrawUI();
		m_infoBG->DrawUI();
		m_infoName->DrawUI();
		m_infoDescription->DrawUI();

		m_infoItem = nullptr;
	}
	
}