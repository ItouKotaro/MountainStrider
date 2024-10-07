//=============================================================
//
// �}�l�[�W���[ [manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "manager.h"
#include "gameobject.h"
#include "scene.h"
#include "input_system.h"
#include "data_field.h"
#include "component/2d/text.h"

//=============================================================
// [CManager] �R���X�g���N�^
//=============================================================
CManager::CManager()
{

}

//=============================================================
// [CManager] �f�X�g���N�^
//=============================================================
CManager::~CManager()
{

}

//=============================================================
// [CManager] ������
//=============================================================
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	// �n���h���̑��
	m_hwnd = hWnd;

	// �����_���[�̐���
	CRenderer::GetInstance()->Init(hInstance, hWnd, bWindow);

	// �t�H���g�̓ǂݍ���
	CText::InitLoadFont();

	// �����̐���
	CPhysics::GetInstance()->Init();

	// �f�[�^�}�l�[�W���[�̐���
	CDataManager::GetInstance()->Init();

	// �I�[�f�B�I�}�l�[�W���[�̐���
	m_pAudioManager = new CAudioManager();
	m_pAudioManager->Init();

	// ���̓V�X�e���̐���
	CInputSystem::GetInstance()->Init(hInstance, hWnd);

	// �V�[���}�l�[�W���[�̐���
	CSceneManager::GetInstance()->Init();
	CSceneManager::GetInstance()->SetScene(CSceneManager::START_SCENE_NAME);

	return S_OK;
}

//=============================================================
// [CManager] �I��
//=============================================================
void CManager::Uninit()
{
	// �V�[���}�l�[�W���[�̏I��
	CSceneManager::GetInstance()->Uninit();

	// ���ׂẴI�u�W�F�N�g���������
	GameObject::DestroyAll(true);
	GameObject::DestroyDeathFlag();

	// ���̓V�X�e���̏I��
	CInputSystem::GetInstance()->Uninit();

	// �����_���[��j������
	CRenderer::GetInstance()->Uninit();

	// ������j������
	CPhysics::GetInstance()->Uninit();

	// �I�[�f�B�I�}�l�[�W���[��j������
	if (m_pAudioManager != nullptr)
	{
		m_pAudioManager->Uninit();
		delete m_pAudioManager;
		m_pAudioManager = nullptr;
	}

	// �f�[�^�Ǘ���j������
	CDataManager::GetInstance()->Uninit();
}

//=============================================================
// [CManager] �X�V
//=============================================================
void CManager::Update()
{
	// ���̓V�X�e���̍X�V
	CInputSystem::GetInstance()->Update();

	// �I�[�f�B�I�̍X�V
	m_pAudioManager->Update();

	// �����_���[�̍X�V
	CRenderer::GetInstance()->Update();

	if (!m_bPause)
	{
		// �����̍X�V
		CPhysics::GetInstance()->Update();
	}

	// �V�[���̍X�V
	CSceneManager::GetInstance()->Update();
}

//=============================================================
// [CManager] �`��
//=============================================================
void CManager::Draw()
{
	// �����_���[�̕`��
	CRenderer::GetInstance()->Draw();

	// �V�[���̕`��
	CSceneManager::GetInstance()->Draw();
}
