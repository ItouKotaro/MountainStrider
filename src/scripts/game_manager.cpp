//=============================================================
//
// �Q�[���}�l�[�W���[ [game_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "game_manager.h"

#include "scripts/status_ui.h"

// �ÓI�����o�ϐ��̏�����
const int CGameManager::MAX_FUEL = 1500;
const int CGameManager::MAX_ENDURANCE = 200;

//=============================================================
// [CGameManager] ������
//=============================================================
void CGameManager::Init()
{
	// �ϐ��̏�����
	m_nFuel = MAX_FUEL;
	m_nEndurance = MAX_ENDURANCE;

	m_pStatusUI = new GameObject("StatusUI", "UI");
	m_pStatusUI->AddComponent<CStatusUI>();
}

//=============================================================
// [CGameManager] �I��
//=============================================================
void CGameManager::Uninit()
{
	m_pStatusUI->Destroy();
}

//=============================================================
// [CGameManager] �X�V
//=============================================================
void CGameManager::Update()
{
	m_pStatusUI->GetComponent<CStatusUI>()->SetFuel(m_nFuel / static_cast<float>(MAX_FUEL));
	m_pStatusUI->GetComponent<CStatusUI>()->SetEndurance(m_nEndurance / static_cast<float>(MAX_ENDURANCE));
}