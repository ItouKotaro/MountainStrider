//=============================================================
//
// オーディオ管理 [audio_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "fmod.hpp"
#include "fmod_studio.hpp"

// オーディオクリップ
typedef FMOD::Sound* AudioClip;

// オーディオバンク
typedef FMOD::Studio::Bank* AudioBank;

// オーディオイベント記述
typedef FMOD::Studio::EventDescription* AudioEventDesc;

// オーディオイベントインスタンス
typedef FMOD::Studio::EventInstance* AudioEventInstance;

// オーディオ管理クラス
class AudioManager
{
public:
	HRESULT Init();			// 初期化
	void Uninit();		// 終了
	void Update();	// 更新

	FMOD::Studio::System* GetSystem() { return m_system; }
	FMOD::System* GetCoreSystem() { return m_coreSystem; }

	static AudioManager* GetInstance()
	{
		static AudioManager instance;
		return &instance;
	}

	//@brief オーディオクリップの作成
	AudioClip CreateClip(std::string filePath, FMOD_MODE mode = FMOD_2D, bool isStream = false);

	//@brief オーディオクリップの削除
	void RemoveClip(AudioClip clip);

	//@brief オーディオバンクの読み込み
	AudioBank LoadBank(std::string filePath);

	//@brief オーディオイベント記述の取得
	AudioEventDesc GetEventDesc(std::string path);
private:
	FMOD::Studio::System* m_system;
	FMOD::System* m_coreSystem;
	std::vector<AudioClip> m_audioClipList;
};

#endif // !_AUDIO_H_
