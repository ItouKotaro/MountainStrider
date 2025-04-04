//=============================================================
//
// ���[�h�Ǘ� [mode_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "mode_manager.h"

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
