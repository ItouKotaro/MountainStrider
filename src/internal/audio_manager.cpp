//=============================================================
//
// オーディオ管理 [audio_manager.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "audio_manager.h"


//=============================================================
// [AudioManager] 初期化
//=============================================================
HRESULT AudioManager::Init()
{
	// システムの作成
	FMOD::Studio::System::create(&m_system);

	// コアシステムを取得する
	m_system->getCoreSystem(&m_coreSystem);
	
	// フォーマット設定
	m_coreSystem->setSoftwareFormat(0, FMOD_SPEAKERMODE_5POINT1, 0);

	// システムの初期化
	void* extraDriverData = nullptr;
	m_system->initialize(1024, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData);

	// 距離単位の設定
	m_coreSystem->set3DSettings(1.0f, 1.0f, 1.0f);



	return S_OK;
}

//=============================================================
// [AudioManager] 終了
//=============================================================
void AudioManager::Uninit()
{
	// サウンドを破棄する
	for (auto itr = m_audioClipList.begin(); itr != m_audioClipList.end(); itr++)
	{
		(*itr)->release();
	}
	m_audioClipList.clear();

	// システムを解放する
	if (m_system != nullptr)
	{
		m_system->release();
	}
}

//=============================================================
// [AudioManager] 更新
//=============================================================
void AudioManager::Update()
{
	if (m_system != nullptr)
	{
		m_system->update();
	}
}

//=============================================================
// [AudioManager] オーディオクリップの作成
//=============================================================
AudioClip AudioManager::CreateClip(std::string filePath, FMOD_MODE mode, bool isStream)
{
	AudioClip sound = nullptr;
	isStream ? m_coreSystem->createStream(filePath.c_str(), mode, 0, &sound) : m_coreSystem->createSound(filePath.c_str(), mode, 0, &sound);
	m_audioClipList.push_back(sound);
	return sound;
}

//=============================================================
// [AudioManager] オーディオクリップの削除
//=============================================================
void AudioManager::RemoveClip(AudioClip clip)
{
	for (auto itr = m_audioClipList.begin(); itr != m_audioClipList.end(); itr++)
	{
		if (*itr == clip)
		{
			m_audioClipList.erase(itr);
			clip->release();
			return;
		}
	}
}

//=============================================================
// [AudioManager] オーディオバンクの読み込み
//=============================================================
AudioBank AudioManager::LoadBank(std::string filePath)
{
	AudioBank bank = nullptr;
	m_system->loadBankFile(filePath.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);
	return bank;
}

//=============================================================
// [AudioManager] オーディオイベント記述の取得
//=============================================================
AudioEventDesc AudioManager::GetEventDesc(std::string path)
{
	AudioEventDesc desc = nullptr;
	m_system->getEvent(path.c_str(), &desc);
	return desc;
}