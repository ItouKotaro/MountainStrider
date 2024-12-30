//=============================================================
//
// サウンド [sound.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _AUDIO_H_
#define _AUDIO_H_

#include "component.h"
#include "internal/audio_manager.h"

// オーディオソース
class AudioSource : public Component
{
public:
	AudioSource() : m_audioClip(nullptr), m_channel(nullptr){}
	void Uninit() override;
	void Update() override;
	void Play(AudioClip audioClip);
	void PlayOneShot(AudioClip audioClip, float volume = 1.0f);
	void SetPause(bool isPause);
	FMOD::Channel* GetChannel() { return m_channel; }
private:
	AudioClip m_audioClip;
	FMOD::Channel* m_channel;
	std::vector<FMOD::Channel*> m_oneShots;
};

// オーディオイベント
class AudioEvent : public Component
{
public:
	AudioEvent() : m_instance(nullptr){}
	void Uninit() override;
	void Update() override;
	void SetEventDesc(AudioEventDesc desc);
	AudioEventInstance GetInstance() { return m_instance; }
private:
	AudioEventInstance m_instance;
};

// オーディオリスナー
class AudioListener : public Component
{
public:
	AudioListener() : m_listenerID(0){}
	void Update() override;
	void SetListenerID(int id) { m_listenerID = id; }
private:
	int m_listenerID;
	D3DXVECTOR3 m_beforePos;
};

#endif // !_AUDIO_H_
