//=============================================================
//
// サウンド [sound.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "audio.h"
#include "manager.h"

//=============================================================
// [AudioSource] 終了
//=============================================================
void AudioSource::Uninit()
{
	if (m_channel != nullptr)
	{
		m_channel->stop();
	}
}

//=============================================================
// [AudioSource] 更新
//=============================================================
void AudioSource::Update()
{
	if (m_channel != nullptr)
	{
		FMOD_VECTOR pos = { transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z };
		FMOD_VECTOR vel = { 0.0f, 0.0f, 0.0f };
		m_channel->set3DAttributes(&pos, &vel);

		// 終了したら破棄する場合
		if (m_isEndDestroy)
		{
			bool isPlaying;
			m_channel->isPlaying(&isPlaying);
			if (!isPlaying)
			{ // 再生されていないとき
				// 破棄する
				gameObject->Destroy();
			}
		}
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
// [AudioSource] 再生
//=============================================================
void AudioSource::Play(AudioClip audioClip)
{
	m_audioClip = audioClip;
	if (m_audioClip != nullptr)
	{
		// チャンネルが既に存在するとき
		if (m_channel != nullptr)
		{
			m_channel->stop();
			m_channel = nullptr;
		}

		// チャンネルを作成し、再生する
		AudioManager::GetInstance()->GetCoreSystem()->playSound(m_audioClip, 0, false, &m_channel);
		Update();
	}
}

//=============================================================
// [AudioSource] 再生
//=============================================================
void AudioSource::PlayOneShot(AudioClip audioClip, float volume)
{
	if (audioClip != nullptr)
	{
		// チャンネルを作成し、再生する
		FMOD::Channel* channel;
		AudioManager::GetInstance()->GetCoreSystem()->playSound(audioClip, 0, false, &channel);
		channel->setVolume(volume);
		m_oneShots.push_back(channel);
	}
}

//=============================================================
// [AudioSource] ポーズ
//=============================================================
void AudioSource::SetPause(bool isPause)
{
	if (m_channel != nullptr)
	{
		m_channel->setPaused(isPause);
	}
}

//=============================================================
// [AudioListener] 更新
//=============================================================
void AudioListener::Update()
{
	// オーディオマネージャーを取得する
	AudioManager* audioManager = AudioManager::GetInstance();

	// リスナーの位置を適用する
	FMOD_VECTOR pos;
	pos.x = transform->GetWPos().x;
	pos.y = transform->GetWPos().y;
	pos.z = transform->GetWPos().z;

	// 前方
	FMOD_VECTOR forward = { 0.0f, 0.0f, 0.0f };
	forward.x = sinf(transform->GetWRot().y);
	forward.y = 0.0f;
	forward.z = cosf(transform->GetWRot().y);

	// 上方向
	FMOD_VECTOR up;
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// 速度
	FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
	velocity.x = (transform->GetWPos().x - m_beforePos.x) * (1000 / static_cast<float>(CManager::GetInstance()->GetFPS()));
	velocity.y = (transform->GetWPos().y - m_beforePos.y) * (1000 / static_cast<float>(CManager::GetInstance()->GetFPS()));
	velocity.z = (transform->GetWPos().z - m_beforePos.z) * (1000 / static_cast<float>(CManager::GetInstance()->GetFPS()));

	// 適用する
	//audioManager->GetCoreSystem()->set3DListenerAttributes(m_listenerID, &pos, &velocity, &forward, &up);

	FMOD_3D_ATTRIBUTES attributes = { {0} };
	attributes.position = pos;
	attributes.forward = forward;
	attributes.velocity = velocity;
	attributes.up = up;
	audioManager->GetSystem()->setListenerAttributes(m_listenerID, &attributes);

	// 前回の位置として記録する
	m_beforePos = transform->GetWPos();
}

//=============================================================
// [AudioEvent] 終了
//=============================================================
void AudioEvent::Uninit()
{
	if (m_instance != nullptr)
	{
		m_instance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT);
		m_instance->release();
	}
}

//=============================================================
// [AudioEvent] 更新
//=============================================================
void AudioEvent::Update()
{
	if (m_instance != nullptr)
	{
		// 位置を適用する
		FMOD_VECTOR pos;
		pos.x = transform->GetWPos().x;
		pos.y = transform->GetWPos().y;
		pos.z = transform->GetWPos().z;

		// 前方
		FMOD_VECTOR forward = { 0.0f, 0.0f, 0.0f };
		D3DXMATRIX rotMtx = transform->GetRotationMatrix();
		D3DXVECTOR3 posR = { 0.0f, 0.0f, 1.0f };
		D3DXVec3TransformCoord(&posR, &posR, &rotMtx);
		posR.y = 0.0f;
		D3DXVec3Normalize(&posR, &posR);
		forward.x = posR.x;
		forward.y = posR.y;
		forward.z = posR.z;

		// 上方向
		FMOD_VECTOR up;
		up.x = 0.0f;
		up.y = 1.0f;
		up.z = 0.0f;

		// 速度
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
// [AudioEvent] 更新
//=============================================================
void AudioEvent::SetEventDesc(AudioEventDesc desc)
{
	if (desc != nullptr)
	{
		// 既にインスタンスが存在するとき
		if (m_instance != nullptr)
		{
			m_instance->release();
		}

		// 作成する
		desc->createInstance(&m_instance);
	}
}