//=============================================================
//
// �T�E���h [sound.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "sound.h"
#include "manager.h"

//=============================================================
// [CSound] ������
//=============================================================
void CSound::Init()
{
	//�ϐ��̏�����
	m_bStoppedDestroy = false;
	m_fadeState = FADESTATE::NONE;
	m_fFadeValue = 0.0f;
	m_fFadeMaxVolume = 0.0f;

	// �\�[�X�̐���
	alGenSources(1, &m_source);
}

//=============================================================
// [CSound] �I��
//=============================================================
void CSound::Uninit()
{
	// �\�[�X�̔j��
	alDeleteSources(1, &m_source);
}

//=============================================================
// [CSound] �X�V
//=============================================================
void CSound::Update()
{
	// �Đ��ʒu��ݒ肷��
	alSource3f(m_source, AL_POSITION, transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z);

	// ��~������j������
	if (m_bStoppedDestroy && GetState() == STATE::STOPPED)
	{
		gameObject->Destroy();
	}

	// ���ʂ��擾
	float fVolume;
	alGetSourcef(m_source, AL_GAIN, &fVolume);

	if (m_fadeState == FADESTATE::FADEIN)
	{ // �t�F�[�h�C��
		if (fVolume + m_fFadeValue < m_fFadeMaxVolume)
		{ // �t�F�[�h�C����
			// ���ʂ����Z
			SetVolume(fVolume + m_fFadeValue);
		}
		else
		{ // ���ʂ�����l�𒴂�����ő�ɕύX����
			// �ő剹�ʂɐݒ�
			SetVolume(m_fFadeMaxVolume);

			// ���Ƃɖ߂�
			m_fadeState = FADESTATE::NONE;
		}
	}
	else if (m_fadeState == FADESTATE::FADEOUT)
	{ // �t�F�[�h�A�E�g
		// ���ʂ�������
		SetVolume(fVolume - m_fFadeValue);

		// ���ʂ�0�ȉ��̂Ƃ��j������
		if (fVolume <= 0.0f)
		{
			gameObject->Destroy();
		}
	}
}

//=============================================================
// [CSound] WAVE�t�@�C����ǂݍ���
//=============================================================
bool CSound::LoadWav(const std::string& sPath)
{
	// �o�b�t�@���擾����
	ALuint bufferID = CDataManager::GetInstance()->RefAudio(sPath)->GetBuffer();

	// �o�b�t�@���\�[�X�ɃZ�b�g����
	alSourcei(m_source, AL_BUFFER, bufferID);

	return true;
}

//=============================================================
// [CSound] �Đ�
//=============================================================
void CSound::Play()
{
	alSourcePlay(m_source);
}

//=============================================================
// [CSound] ��~
//=============================================================
void CSound::Stop()
{
	alSourceStop(m_source);
}

//=============================================================
// [CSound] �ꎞ��~
//=============================================================
void CSound::Pause()
{
	alSourcePause(m_source);
}

//=============================================================
// [CSound] �����߂�
//=============================================================
void CSound::Rewind()
{
	alSourceRewind(m_source);
}

//=============================================================
// [CSound] ���[�v
//=============================================================
void CSound::SetLoop(const bool& bEnable)
{
	if (bEnable)
		alSourcei(m_source, AL_LOOPING, AL_TRUE);
	else
		alSourcei(m_source, AL_LOOPING, AL_FALSE);
}

//=============================================================
// [CSound] ����
//=============================================================
void CSound::SetVolume(const float& fVolume)
{
	alSourcef(m_source, AL_GAIN, fVolume);
}

//=============================================================
// [CSound] �s�b�`
//=============================================================
void CSound::SetPitch(const float& fPitch)
{
	alSourcef(m_source, AL_PITCH, fPitch);
}

//=============================================================
// [CSound] �t�F�[�h�A�E�g
//=============================================================
void CSound::FadeOut(const float& fFadeValue)
{
	m_fadeState = FADESTATE::FADEOUT;
	m_fFadeValue = fFadeValue;
}

//=============================================================
// [CSound] �t�F�[�h�C��
//=============================================================
void CSound::FadeIn(const float& fFadeValue, const float& fMaxVolume)
{
	m_fadeState = FADESTATE::FADEIN;
	m_fFadeValue = fFadeValue;
	m_fFadeMaxVolume = fMaxVolume;
}

//=============================================================
// [CSound] ��Ԃ̎擾
//=============================================================
CSound::STATE CSound::GetState()
{
	int nState = 0;
	alGetSourcei(m_source, AL_SOURCE_STATE, &nState);

	switch (nState)
	{
	case AL_INITIAL:
		return STATE::INITIAL;
	case AL_PLAYING:
		return STATE::PLAYING;
	case AL_PAUSED:
		return STATE::PAUSED;
	case AL_STOPPED:
		return STATE::STOPPED;
	}

	return STATE::INITIAL;
}

//=============================================================
// [CSound] �I��������I�u�W�F�N�g��j������
//=============================================================
void CSound::IsStoppedDestroy(const bool& bEnable)
{
	m_bStoppedDestroy = bEnable;
}


//=============================================================
// [CSoundListener] ������
//=============================================================
void CSoundListener::Init()
{
	// �O��̈ʒu�����̈ʒu�ɐݒ肷��
	m_oldPos = transform->GetWPos();
}

//=============================================================
// [CSoundListener] �I��
//=============================================================
void CSoundListener::Uninit()
{
	// ���X�i�[�ݒ��������Ԃɖ߂�
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	float fAngle = 0.0f;
	alListenerfv(AL_ORIENTATION, &fAngle);
}

//=============================================================
// [CSoundListener] �X�V
//=============================================================
void CSoundListener::Update()
{
	// �ʒu��ݒ肷��
	alListener3f(AL_POSITION, transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z);

	// ������ݒ肷��
	if (m_bUpdateAngle)
	{
		float fAngle = transform->GetWRotY();
		alListenerfv(AL_ORIENTATION, &fAngle);
	}
}

//=============================================================
// [CSoundListener] ������ݒ肷��
//=============================================================
void CSoundListener::SetAngle(const float& fAngle)
{
	alListenerfv(AL_ORIENTATION, &fAngle);
}