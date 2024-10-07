//=============================================================
//
// サウンド [sound.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "component.h"
#include <fstream>

// サウンドコンポーネント
class CSound : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	bool LoadWav(const std::string& sPath);

	// 状態
	enum STATE
	{
		INITIAL,	// 再生前
		PLAYING,	// 再生中
		PAUSED,	// 一時停止中
		STOPPED,	// 再生終了後
	};

	// フェード状態
	enum FADESTATE
	{
		NONE,		// なし
		FADEIN,		// フェードイン
		FADEOUT	// フェードアウト
	};

	void Play();
	void Stop();
	void Pause();
	void Rewind();
	void SetLoop(const bool& bEnable);
	void SetVolume(const float& fVolume);
	void SetPitch(const float& fPitch);
	void FadeOut(const float& fFadeValue = 0.01f);
	void FadeIn(const float& fFadeValue = 0.01f, const float& fMaxVolume = 1.0f);
	STATE GetState();
	void IsStoppedDestroy(const bool& bEnable = true);
private:
	ALuint m_source;					// ソースID
	bool m_bStoppedDestroy;		// 停止したときにオブジェクトを破棄するか

	float m_fFadeValue;				// フェード値
	float m_fFadeMaxVolume;		// フェードの最大音量（フェードインに使用）
	FADESTATE m_fadeState;		// フェード状態
};

// リスナー
class CSoundListener : public Component
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

	void IsUpdateAngle(const bool& bEnable) { m_bUpdateAngle = bEnable; }
	void SetAngle(const float& fAngle);
private:
	D3DXVECTOR3 m_oldPos;
	bool m_bUpdateAngle;
};

#endif // !_SOUND_H_
