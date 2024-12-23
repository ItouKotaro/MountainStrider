//=============================================================
//
// �X�e�[�^�XUI [status_ui.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "status_ui.h"

//=============================================================
// [CStatusUI] ������
//=============================================================
void CStatusUI::Init()
{
	GameObject* pFuelObj = new GameObject;
	GameObject* pEnduranceObj = new GameObject;
	m_fuelIcon = new SingleComponent<CPolygon>();
	m_enduranceIcon = new SingleComponent<CPolygon>();
	m_fuelIcon->Init();
	m_enduranceIcon->Init();
	
	// �A�C�R���̐ݒ�
	m_fuelIcon->SetParent(pFuelObj);
	m_enduranceIcon->SetParent(pEnduranceObj);
	m_fuelIcon->transform->SetPos(-60.0f, -25.0f);
	m_enduranceIcon->transform->SetPos(-60.0f, -25.0f);
	m_fuelIcon->transform->SetSize(45.0f, 45.0f);
	m_enduranceIcon->transform->SetSize(45.0f, 45.0f);
	m_fuelIcon->SetTexture("data\\TEXTURE\\UI\\fuel_icon.png");
	m_enduranceIcon->SetTexture("data\\TEXTURE\\UI\\endurance_icon.png");

	// �I�u�W�F�N�g�𐶐�����
	m_pFuelBar = pFuelObj->AddComponent<CAdvancedBar>();
	m_pEnduranceBar = pEnduranceObj->AddComponent<CAdvancedBar>();

	// �o�[�̈ʒu��ύX����
	pFuelObj->transform->SetPos(100.0f, 60.0f);
	pEnduranceObj->transform->SetPos(100.0f, 120.0f);

	// �o�[�̐ݒ�
	m_pFuelBar->SetColor(0, D3DCOLOR_RGBA(252, 185, 40, 255));
	m_pFuelBar->SetColor(2, D3DCOLOR_RGBA(252, 185, 40, 255));
	m_pFuelBar->SetColor(1, D3DCOLOR_RGBA(255, 0, 0, 255));
	m_pFuelBar->SetColor(3, D3DCOLOR_RGBA(255, 0, 0, 255));

	m_pEnduranceBar->SetColor(0, D3DCOLOR_RGBA(7, 232, 104, 255));
	m_pEnduranceBar->SetColor(2, D3DCOLOR_RGBA(7, 232, 104, 255));
	m_pEnduranceBar->SetColor(1, D3DCOLOR_RGBA(10, 201, 163, 255));
	m_pEnduranceBar->SetColor(3, D3DCOLOR_RGBA(10, 201, 163, 255));
}

//=============================================================
// [CStatusUI] �I��
//=============================================================
void CStatusUI::Uninit()
{
	m_pFuelBar->gameObject->Destroy();
	m_pEnduranceBar->gameObject->Destroy();
	m_fuelIcon->Uninit();
	delete m_fuelIcon;
	m_enduranceIcon->Uninit();
	delete m_enduranceIcon;
}

//=============================================================
// [CStatusUI] �X�V
//=============================================================
void CStatusUI::Update()
{
	m_fuelIcon->Update();
	m_enduranceIcon->Update();
}

//=============================================================
// [CStatusUI] UI�`��
//=============================================================
void CStatusUI::DrawUI()
{
	m_fuelIcon->DrawUI();
	m_enduranceIcon->DrawUI();
}

//=============================================================
// [CStatusUI] �R���̐ݒ�
//=============================================================
void CStatusUI::SetFuel(const float& percent)
{
	if (percent != m_pFuelBar->GetProgress())
	{
		m_pFuelBar->SetProgress(percent);
	}
}

//=============================================================
// [CStatusUI] �ϋv�l�̐ݒ�
//=============================================================
void CStatusUI::SetEndurance(const float& percent)
{
	if (percent != m_pEnduranceBar->GetProgress())
	{
		m_pEnduranceBar->SetProgress(percent);
	}
}

//=============================================================
// [CStatusUI] �\���ݒ�
//=============================================================
void CStatusUI::SetVisible(const bool& visible)
{
	m_pFuelBar->gameObject->SetVisible(visible);
	m_pEnduranceBar->gameObject->SetVisible(visible);
	m_fuelIcon->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, visible ? 1.0f : 0.0f));
	m_enduranceIcon->SetColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, visible ? 1.0f : 0.0f));
}