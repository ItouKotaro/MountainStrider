//=============================================================
//
// ���[�h�Ǘ� [mode_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "mode_manager.h"
#include "manager.h"
#include "scene/game.h"

//=============================================================
// [ModeManager] ������
//=============================================================
void ModeManager::Init()
{
	if (m_mode != nullptr)
	{
		m_mode->Init();
	}
}

//=============================================================
// [ModeManager] �I��
//=============================================================
void ModeManager::Uninit()
{
	if (m_mode != nullptr)
	{
		m_mode->Uninit();
	}

	if (m_result != nullptr)
	{
		m_result->Uninit();
		delete m_result;
		m_result = nullptr;
	}
}

//=============================================================
// [ModeManager] �X�V
//=============================================================
void ModeManager::Update()
{
	if (m_mode != nullptr)
	{
		m_mode->Update();
	}
}

//=============================================================
// [ModeManager] �Ō�̏I��
//=============================================================
void ModeManager::LastUninit()
{
	if (m_mode != nullptr)
	{
		delete m_mode;
		m_mode = nullptr;
	}
}

//=============================================================
// [ModeManager] ���[�h�ݒ�
//=============================================================
void ModeManager::SetMode(ModeTemplate* mode)
{
	// ���Ƀ��[�h���ݒ肳��Ă���ꍇ�͏I������
	if (m_mode != nullptr)
	{
		m_mode->Uninit();
		delete m_mode;
		m_mode = nullptr;
	}

	// �V�������[�h��ݒ肷��
	m_mode = mode;
}

//=============================================================
// [ModeManager] ���U���g��ݒ肷��
//=============================================================
void ModeManager::SetResult(ResultBase* result)
{
	// ���U���g������Ƃ��͍폜����
	if (m_result != nullptr)
	{
		m_result->Uninit();
		delete m_result;
		m_result = nullptr;
	}

	// ���U���g�f�[�^���v�Z����
	auto gameScene = static_cast<CGameScene*>(CSceneManager::GetInstance()->GetScene("game")->pScene);
	gameScene->CalcResultData();

	// ���U���g��ݒ肷��
	m_result = result;
	m_result->Init();
}
