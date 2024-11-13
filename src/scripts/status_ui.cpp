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

	// �I�u�W�F�N�g�𐶐�����
	m_pFuelBar = pFuelObj->AddComponent<CAdvancedBar>();
	m_pEnduranceBar = pEnduranceObj->AddComponent<CAdvancedBar>();

	// �o�[�̈ʒu��ύX����
	pFuelObj->transform->SetPos(50.0f, 100.0f);
	pEnduranceObj->transform->SetPos(50.0f, 150.0f);

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