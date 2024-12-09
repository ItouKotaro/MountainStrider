//=============================================================
//
// �V���b�v [shop.cpp]
// Author: Ito Kotaro
// 
//=============================================================
#include "shop.h"

#include "renderer.h"
#include "scripts/result/mt_result.h"
#include "scene/game.h"
#include "scripts/vehicle.h"

#include "component/2d/polygon.h"
#include "component/2d/text.h"
#include "scripts/result/result_view.h"
#include "component/other/button.h"
#include "scripts/shop/buy_button.h"
#include "scripts/shop/inventory.h"

const float ShopManager::BAR_SPACE = 110.0f;
int ShopManager::m_points = 100;

//=============================================================
// [ShopManager] ������
//=============================================================
void ShopManager::Init(Pages* pages)
{
	m_pages = pages;
	m_viewPoints = m_points;
	m_pointsCounter = 0;

	// �A�C�e���̃��X�g�ǉ�����
	RegisterItemList();

	// �Q�[���V�[���̎擾
	m_gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	// �|�C���g�\��
	m_ptView = new GameObject();
	m_ptView->SetPriority(6);
	m_ptView->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH), static_cast<float>(CRenderer::SCREEN_HEIGHT));
	m_ptView->AddComponent<CPolygon>();
	m_ptView->GetComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SHOP\\shop.png");
	m_pages->AddObject(1, m_ptView);

	// �|�C���g�̐����e�L�X�g
	m_ptText = new GameObject();
	m_ptText->transform->SetPos(CRenderer::SCREEN_WIDTH / 2 + 50.0f, 100.0f);
	m_ptText->SetPriority(7);
	m_ptText->AddComponent<CText>()->SetText("<color=23,116,255>"+std::to_string(m_points));
	m_ptText->GetComponent<CText>()->SetAlign(CText::CENTER);
	m_ptText->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
	m_ptText->GetComponent<CText>()->SetFontSize(120);
	m_pages->AddObject(1, m_ptText);

	// �V���b�v�A�C�e���{�^�����쐬����
	for (int i = 0; i < 6; i++)
	{
		m_shopItems[i] = new GameObject();
		m_shopItems[i]->AddComponent<BuyButtonUI>(m_itemList[1]);
		m_shopItems[i]->transform->SetPos(300.0f * i + 40.0f, 350.0f);
		m_shopItems[i]->transform->Translate(i < 3 ? 0.0f : 60.0f, 0.0f, 0.0f);
		m_pages->AddObject(1, m_shopItems[i]);
	}

	// �C���x���g�����쐬����
	m_inventory = new GameObject("Inventory", "UI");
	m_inventory->AddComponent<InventoryUI>();
	m_inventory->transform->SetPos(static_cast<float>(CRenderer::SCREEN_WIDTH / 2) - 450.0f, 820.0f);
	m_pages->AddObject(1, m_inventory);

	// �R���Ƒϋv�l�̍w���f�B�X�v���C������������
	InitTopDisplay();
}

//=============================================================
// [ShopManager] �f�B�X�v���C�̏�����
//=============================================================
void ShopManager::InitTopDisplay()
{
	// �Q�[���V�[���̎擾
	m_gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	for (int idx = 0; idx < 2; idx++)
	{
		// �����I�u�W�F�N�g
		GameObject* uniteObj = new GameObject();
		m_pages->AddObject(1, uniteObj);
		uniteObj->transform->SetPos(static_cast<float>(idx == 0 ? -CRenderer::SCREEN_WIDTH / 2 : CRenderer::SCREEN_WIDTH), 0.0f);
		if (idx == 0) m_fuel = uniteObj;
		else m_endurance = uniteObj;

		// �w�i
		GameObject* bgObj = new GameObject();
		bgObj->SetParent(uniteObj);
		bgObj->SetPriority(5);
		bgObj->AddComponent<CPolygon>()->SetColor(idx == 0 ? D3DCOLOR_RGBA(201, 24, 0, 255) : D3DCOLOR_RGBA(15, 92, 15, 255));
		bgObj->GetComponent<CPolygon>()->SetColor(1, idx == 0 ? D3DCOLOR_RGBA(250, 176, 97, 255) : D3DCOLOR_RGBA(24, 184, 90, 255));
		bgObj->GetComponent<CPolygon>()->SetColor(3, idx == 0 ? D3DCOLOR_RGBA(250, 176, 97, 255) : D3DCOLOR_RGBA(24, 184, 90, 255));
		bgObj->transform->SetSize(static_cast<float>(CRenderer::SCREEN_WIDTH / 2), 280.0f);
		m_pages->AddObject(1, bgObj);

		// �e�L�X�g
		GameObject* textObj = new GameObject();
		textObj->transform->SetPos(idx == 0 ? 120.0f : 340.0f, 40.0f);
		textObj->SetParent(uniteObj);
		textObj->SetPriority(6);
		textObj->AddComponent<CText>()->SetText(idx == 0 ? "�R��" : "�ϋv�l");
		textObj->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		textObj->GetComponent<CText>()->SetFontSize(80);
		m_pages->AddObject(1, textObj);

		// �o�[
		GameObject* barObj = new GameObject();
		barObj = new GameObject();
		barObj->SetParent(uniteObj);
		barObj->SetPriority(7);
		m_pages->AddObject(1, barObj);
		barObj->transform->SetPos(idx == 0 ? CRenderer::SCREEN_WIDTH / 4 - BAR_SPACE : CRenderer::SCREEN_WIDTH / 4 + BAR_SPACE, 150.0f);
		barObj->AddComponent<CAdvancedBar>();
		barObj->GetComponent<CAdvancedBar>()->SetAlign(CAdvancedBar::CENTER);
		barObj->GetComponent<CAdvancedBar>()->SetLength(500.0f);
		barObj->GetComponent<CAdvancedBar>()->SetBold(30.0f);
		barObj->GetComponent<CAdvancedBar>()->SetColor(D3DCOLOR_RGBA(0, 0, 0, 255));
		barObj->GetComponent<CAdvancedBar>()->SetTexture("data\\TEXTURE\\RESULT\\hand_bar.png");
		barObj->GetComponent<CAdvancedBar>()->SetProgress(idx == 0 ? pBike->GetFuel() / static_cast<float>(CVehicle::MAX_FUEL) : pBike->GetEndurance() / static_cast<float>(CVehicle::MAX_ENDURANCE));
		if (idx == 0) m_fuelBar = barObj;
		else m_enduranceBar = barObj;

		// ���ݒl�ƍő�l
		GameObject* currentValueObj = new GameObject();
		currentValueObj->SetParent(uniteObj);
		currentValueObj->SetPriority(7);
		m_pages->AddObject(1, currentValueObj);
		currentValueObj->transform->SetPos(idx == 0 ? 620.0f : 837.5f, 90.0f);
		currentValueObj->AddComponent<CText>()->SetText(std::to_string(static_cast<int>(idx == 0 ? pBike->GetFuel() : pBike->GetEndurance())) + "<size=20>/" + std::to_string(static_cast<int>(idx == 0 ? CVehicle::MAX_FUEL : CVehicle::MAX_ENDURANCE)));
		currentValueObj->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		currentValueObj->GetComponent<CText>()->SetFontSize(40);
		currentValueObj->GetComponent<CText>()->SetAlign(CText::RIGHT);
		if (idx == 0) m_fuelCurrentValue = currentValueObj;
		else m_enduranceCurrentValue = currentValueObj;

		// �w���{�^��
		GameObject* buttonObj = new GameObject();
		buttonObj->SetParent(uniteObj);
		m_pages->AddObject(1, buttonObj);
		buttonObj->transform->SetSize(300.0f, 90.0f);
		buttonObj->transform->SetPos(idx == 0 ? D3DXVECTOR2(230.0f, 180.0f) : D3DXVECTOR2(440.0f, 180.0f));
		buttonObj->AddComponent<ButtonUI>();
		buttonObj->GetComponent<ButtonUI>()->SetTexture("data\\TEXTURE\\SHOP\\maintenance_buy.png");
		buttonObj->GetComponent<ButtonUI>()->setClickEvent([this, idx]() {
			if (idx ==0) m_fuelItem.onBuy();
			else m_enduranceItem.onBuy();
			});
		if (idx == 0) m_fuelButton = buttonObj;
		else m_enduranceButton = buttonObj;

		// �R�X�g�A�C�R��
		GameObject* costIconObj = new GameObject();
		costIconObj->SetParent(uniteObj);
		costIconObj->transform->SetPos(idx == 0 ? D3DXVECTOR2(235.0f, 188.0f) : D3DXVECTOR2(445.0f, 188.0f));
		costIconObj->transform->SetSize(40.0f, 40.0f);
		costIconObj->AddComponent<CPolygon>()->SetTexture("data\\TEXTURE\\SHOP\\point.png");
		m_pages->AddObject(1, costIconObj);

		// �R�X�g�\��
		GameObject* costObj = new GameObject();
		costObj->SetParent(costIconObj);
		costObj->transform->SetPos(60.0f, 9.0f);
		costObj->AddComponent<CText>()->SetText(std::to_string(idx==0?m_fuelItem.GetPrice():m_enduranceItem.GetPrice()));
		costObj->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		costObj->GetComponent<CText>()->SetFontSize(36);
		costObj->GetComponent<CText>()->SetAlign(CText::CENTER);
		if (idx == 0) m_fuelCost = costObj;
		else m_enduranceCost = costObj;
		m_pages->AddObject(1, costObj);

		// �����ʕ\��
		GameObject* buttonTextObj = new GameObject();
		buttonTextObj->SetParent(buttonObj);
		buttonTextObj->transform->SetPos(195.0f, 23.0f);
		buttonTextObj->AddComponent<CText>()->SetText("+"+std::to_string(idx==0?m_fuelItem.GetAmount():m_enduranceItem.GetAmount()));
		buttonTextObj->GetComponent<CText>()->SetFont("07�S�r�S�V�b�N");
		buttonTextObj->GetComponent<CText>()->SetFontSize(60);
		buttonTextObj->GetComponent<CText>()->SetAlign(CText::CENTER);
		if (idx == 0) m_fuelButtonText = buttonTextObj;
		else m_enduranceButtonText = buttonTextObj;
		m_pages->AddObject(1, buttonTextObj);
	}
}

//=============================================================
// [ShopManager] �I��
//=============================================================
void ShopManager::Uninit()
{
	// �A�C�e�����X�g�ƃp�[�N���X�g��j������
	for (auto itr = m_itemList.begin(); itr != m_itemList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
		}
	}
	m_itemList.clear();

	for (auto itr = m_perkList.begin(); itr != m_perkList.end(); itr++)
	{
		if (*itr != nullptr)
		{
			delete* itr;
		}
	}
	m_perkList.clear();
}

//=============================================================
// [ShopManager] �X�V
//=============================================================
void ShopManager::Update()
{
	auto pBike = m_gameScene->GetBike()->GetComponent<CVehicle>();

	// �|�C���g�̍X�V
	if (m_viewPoints < m_points) m_viewPoints++;
	else if (m_viewPoints > m_points) m_viewPoints--;
	m_ptText->GetComponent<CText>()->SetText("<color=23,116,255>" + std::to_string(m_viewPoints));

	// �R���Ƒϋv�l�̃A�j���[�V����
	m_fuel->transform->Translate((D3DXVECTOR3(0.0f, 0.0f, 0.0f) - m_fuel->transform->GetWPos()) * 0.08f);
	m_endurance->transform->Translate((D3DXVECTOR3(static_cast<float>(CRenderer::SCREEN_WIDTH/2), 0.0f, 0.0f) - m_endurance->transform->GetWPos()) * 0.08f);

	// �o�[�̍X�V
	m_fuelBar->GetComponent<CAdvancedBar>()->SetProgress(pBike->GetFuel() / static_cast<float>(CVehicle::MAX_FUEL));
	m_fuelBar->GetComponent<CAdvancedBar>()->UpdateVertex();
	m_fuelCurrentValue->GetComponent<CText>()->SetText(std::to_string(static_cast<int>(pBike->GetFuel())) + "<size=20>/" + std::to_string(static_cast<int>(CVehicle::MAX_FUEL)));
	
	m_enduranceBar->GetComponent<CAdvancedBar>()->SetProgress(pBike->GetEndurance() / static_cast<float>(CVehicle::MAX_ENDURANCE));
	m_enduranceBar->GetComponent<CAdvancedBar>()->UpdateVertex();
	m_enduranceCurrentValue->GetComponent<CText>()->SetText(std::to_string(static_cast<int>(pBike->GetEndurance())) + "<size=20>/" + std::to_string(static_cast<int>(CVehicle::MAX_ENDURANCE)));

}

//=============================================================
// [ShopManager] �`��
//=============================================================
void ShopManager::Draw()
{
}

//=============================================================
// [ShopManager] ���Z�b�g
//=============================================================
void ShopManager::Reset()
{
	m_points = 30;
	ShopItem::SetSaleDiscount(0.0f);
}

//=============================================================
// [ShopManager] �A�C�e���̓o�^
//=============================================================
void ShopManager::RegisterItemList()
{
	// �A�C�e���̓o�^
	m_itemList.push_back(new ShopFuelTank);
	m_itemList.push_back(new ShopToolBox);

	// �p�[�N�̓o�^
}

//=============================================================
// [ShopManager] �|�C���g���x����
//=============================================================
bool ShopManager::PayPoint(const int& cost)
{
	if (cost <= m_points)
	{ // �x�����鎞
		m_points -= cost;
		return true;
	}
	return false;
}