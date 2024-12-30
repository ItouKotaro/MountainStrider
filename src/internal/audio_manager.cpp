//=============================================================
//
// �I�[�f�B�I�Ǘ� [audio_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "audio_manager.h"


//=============================================================
// [AudioManager] ������
//=============================================================
HRESULT AudioManager::Init()
{
	// �V�X�e���̍쐬
	FMOD::Studio::System::create(&m_system);

	// �R�A�V�X�e�����擾����
	m_system->getCoreSystem(&m_coreSystem);
	
	// �t�H�[�}�b�g�ݒ�
	m_coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

	// �V�X�e���̏�����
	void* extraDriverData = nullptr;
	m_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

	// �����P�ʂ̐ݒ�
	m_coreSystem->set3DSettings(1.0f, 1.0f, 1.0f);



	return S_OK;
}

//=============================================================
// [AudioManager] �I��
//=============================================================
void AudioManager::Uninit()
{
	// �T�E���h��j������
	for (auto itr = m_audioClipList.begin(); itr != m_audioClipList.end(); itr++)
	{
		(*itr)->release();
	}
	m_audioClipList.clear();

	// �V�X�e�����������
	if (m_system != nullptr)
	{
		m_system->release();
	}
}

//=============================================================
// [AudioManager] �X�V
//=============================================================
void AudioManager::Update()
{
	if (m_system != nullptr)
	{
		m_system->update();
	}
}

//=============================================================
// [AudioManager] �I�[�f�B�I�N���b�v�̍쐬
//=============================================================
AudioClip AudioManager::CreateClip(std::string filePath, FMOD_MODE mode, bool isStream)
{
	AudioClip sound = nullptr;
	isStream ? m_coreSystem->createStream(filePath.c_str(), mode, 0, &sound) : m_coreSystem->createSound(filePath.c_str(), mode, 0, &sound);
	m_audioClipList.push_back(sound);
	return sound;
}

//=============================================================
// [AudioManager] �I�[�f�B�I�o���N�̓ǂݍ���
//=============================================================
AudioBank AudioManager::LoadBank(std::string filePath)
{
	AudioBank bank = nullptr;
	m_system->loadBankFile(filePath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
	return bank;
}

//=============================================================
// [AudioManager] �I�[�f�B�I�C�x���g�L�q�̎擾
//=============================================================
AudioEventDesc AudioManager::GetEventDesc(std::string path)
{
	AudioEventDesc desc = nullptr;
	m_system->getEvent(path.c_str(), &desc);
	return desc;
}