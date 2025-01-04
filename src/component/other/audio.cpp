//=============================================================
//
// �T�E���h [sound.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "audio.h"
#include "manager.h"

//=============================================================
// [AudioSource] �I��
//=============================================================
void AudioSource::Uninit()
{
	if (m_channel != nullptr)
	{
		m_channel->stop();
	}
}

//=============================================================
// [AudioSource] �X�V
//=============================================================
void AudioSource::Update()
{
	if (m_channel != nullptr)
	{
		FMOD_VECTOR pos = { transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
		m_channel->set3DAttributes(&pos, &vel);
	}

	for (auto itr = m_oneShots.begin(); itr != m_oneShots.end(); itr++)
	{
		bool isPlaying;
		(*itr)->isPlaying(&isPlaying);
		if (!isPlaying)
		{
			itr = m_oneShots.erase(itr);
			if (itr == m_oneShots.end()) break;
		}
	}
}

//=============================================================
// [AudioSource] �Đ�
//=============================================================
void AudioSource::Play(AudioClip audioClip)
{
	m_audioClip = audioClip;
	if (m_audioClip != nullptr)
	{
		// �`�����l�������ɑ��݂���Ƃ�
		if (m_channel != nullptr)
		{
			m_channel->stop();
			m_channel = nullptr;
		}

		// �`�����l�����쐬���A�Đ�����
		AudioManager::GetInstance()->GetCoreSystem()->playSound(m_audioClip, 0, false, &m_channel);
	}
}

//=============================================================
// [AudioSource] �Đ�
//=============================================================
void AudioSource::PlayOneShot(AudioClip audioClip, float volume)
{
	if (audioClip != nullptr)
	{
		// �`�����l�����쐬���A�Đ�����
		FMOD::Channel* channel;
		AudioManager::GetInstance()->GetCoreSystem()->playSound(audioClip, 0, false, &channel);
		channel->setVolume(volume);
		m_oneShots.push_back(channel);
	}
}

//=============================================================
// [AudioSource] �|�[�Y
//=============================================================
void AudioSource::SetPause(bool isPause)
{
	if (m_channel != nullptr)
	{
		m_channel->setPaused(isPause);
	}
}

//=============================================================
// [AudioListener] �X�V
//=============================================================
void AudioListener::Update()
{
	// �I�[�f�B�I�}�l�[�W���[���擾����
	AudioManager* audioManager = AudioManager::GetInstance();

	// ���X�i�[�̈ʒu��K�p����
	FMOD_VECTOR pos;
	pos.x = transform->GetWPos().x;
	pos.y = transform->GetWPos().y;
	pos.z = transform->GetWPos().z;

	// �O��
	FMOD_VECTOR forward = { 0.0f, 0.0f, 0.0f };
	D3DXMATRIX rotMtx = transform->GetRotationMatrix();
	D3DXVECTOR3 posR = { 0.0f, 0.0f, 1.0f };
	D3DXVec3TransformCoord(&posR, &posR, &rotMtx);
	D3DXVec3Normalize(&posR, &posR);
	forward.x = posR.x;
	forward.y = 0.0f;
	forward.z = posR.z;

	// �����
	FMOD_VECTOR up;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// ���x
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	velocity.x = (transform->GetWPos().x - m_beforePos.x) * (1000 / static_cast<float>(CManager::GetInstance()->GetFPS()));
	velocity.y = (transform->GetWPos().y - m_beforePos.y) * (1000 / static_cast<float>(CManager::GetInstance()->GetFPS()));
	velocity.z = (transform->GetWPos().z - m_beforePos.z) * (1000 / static_cast<float>(CManager::GetInstance()->GetFPS()));

	// �K�p����
	//audioManager->GetCoreSystem()->set3DListenerAttributes(m_listenerID, &pos, &velocity, &forward, &up);

	FMOD_3D_ATTRIBUTES attributes = { {0} };
	attributes.position = pos;
	attributes.forward = forward;
	attributes.velocity = velocity;
	attributes.up = up;
	audioManager->GetSystem()->setListenerAttributes(m_listenerID, &attributes);

	// �O��̈ʒu�Ƃ��ċL�^����
	m_beforePos = transform->GetWPos();
}

//=============================================================
// [AudioEvent] �I��
//=============================================================
void AudioEvent::Uninit()
{
	if (m_instance != nullptr)
	{
		m_instance->release();
	}
}

//=============================================================
// [AudioEvent] �X�V
//=============================================================
void AudioEvent::Update()
{
	if (m_instance != nullptr)
	{
		// �ʒu��K�p����
		FMOD_VECTOR pos;
		pos.x = transform->GetWPos().x;
		pos.y = transform->GetWPos().y;
		pos.z = transform->GetWPos().z;

		// �O��
		FMOD_VECTOR forward = { 0.0f, 0.0f, 0.0f };
		D3DXMATRIX rotMtx = transform->GetRotationMatrix();
		D3DXVECTOR3 posR = { 0.0f, 0.0f, 1.0f };
		D3DXVec3TransformCoord(&posR, &posR, &rotMtx);
		posR.y = 0.0f;
		D3DXVec3Normalize(&posR, &posR);
		forward.x = posR.x;
		forward.y = posR.y;
		forward.z = posR.z;

		// �����
		FMOD_VECTOR up;
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		// ���x
		FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

		FMOD_3D_ATTRIBUTES attributes = { {0} };
		attributes.position = pos;
		attributes.forward = forward;
		attributes.velocity = velocity;
		attributes.up = up;

		m_instance->set3DAttributes(&attributes);
	}
}

//=============================================================
// [AudioEvent] �X�V
//=============================================================
void AudioEvent::SetEventDesc(AudioEventDesc desc)
{
	if (desc != nullptr)
	{
		// ���ɃC���X�^���X�����݂���Ƃ�
		if (m_instance != nullptr)
		{
			m_instance->release();
		}

		// �쐬����
		desc->createInstance(&m_instance);
	}
}