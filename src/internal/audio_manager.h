//=============================================================
//
// �I�[�f�B�I�Ǘ� [audio_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "fmod.hpp"
#include "fmod_studio.hpp"

// �I�[�f�B�I�N���b�v
typedef FMOD::Sound* AudioClip;

// �I�[�f�B�I�o���N
typedef FMOD::Studio::Bank* AudioBank;

// �I�[�f�B�I�C�x���g�L�q
typedef FMOD::Studio::EventDescription* AudioEventDesc;

// �I�[�f�B�I�C�x���g�C���X�^���X
typedef FMOD::Studio::EventInstance* AudioEventInstance;

// �I�[�f�B�I�Ǘ��N���X
class AudioManager
{
public:
	HRESULT Init();			// ������
	void Uninit();		// �I��
	void Update();	// �X�V

	FMOD::Studio::System* GetSystem() { return m_system; }
	FMOD::System* GetCoreSystem() { return m_coreSystem; }

	static AudioManager* GetInstance()
	{
		static AudioManager instance;
		return &instance;
	}

	//@brief �I�[�f�B�I�N���b�v�̍쐬
	AudioClip CreateClip(std::string filePath, FMOD_MODE mode = FMOD_3D, bool isStream = false);

	//@brief �I�[�f�B�I�o���N�̓ǂݍ���
	AudioBank LoadBank(std::string filePath);

	//@brief �I�[�f�B�I�C�x���g�L�q�̎擾
	AudioEventDesc GetEventDesc(std::string path);
private:
	FMOD::Studio::System* m_system;
	FMOD::System* m_coreSystem;
	std::vector<AudioClip> m_audioClipList;
};

#endif // !_AUDIO_H_
