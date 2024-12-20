//=============================================================
//
// サウンド [sound.cpp]
// Author: Ito Kotaro
//
//=============================================================
#include "sound.h"
#include "manager.h"

//=============================================================
// [CSound] 初期化
//=============================================================
void CSound::Init()
{
	//変数の初期化
	m_bStoppedDestroy = false;
	m_fadeState = FADESTATE::NONE;
	m_fFadeValue = 0.0f;
	m_fFadeMaxVolume = 0.0f;

	// ソースの生成
	alGenSources(1, &m_source);
}

//=============================================================
// [CSound] 終了
//=============================================================
void CSound::Uninit()
{
	// ソースの破棄
	alDeleteSources(1, &m_source);
}

//=============================================================
// [CSound] 更新
//=============================================================
void CSound::Update()
{
	// 再生位置を設定する
	alSource3f(m_source, AL_POSITION, transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z);

	// 停止したら破棄する
	if (m_bStoppedDestroy && GetState() == STATE::STOPPED)
	{
		gameObject->Destroy();
	}

	// 音量を取得
	float fVolume;
	alGetSourcef(m_source, AL_GAIN, &fVolume);

	if (m_fadeState == FADESTATE::FADEIN)
	{ // フェードイン
		if (fVolume + m_fFadeValue < m_fFadeMaxVolume)
		{ // フェードイン中
			// 音量を加算
			SetVolume(fVolume + m_fFadeValue);
		}
		else
		{ // 音量が上限値を超えたら最大に変更する
			// 最大音量に設定
			SetVolume(m_fFadeMaxVolume);

			// もとに戻す
			m_fadeState = FADESTATE::NONE;
		}
	}
	else if (m_fadeState == FADESTATE::FADEOUT)
	{ // フェードアウト
		// 音量を下げる
		SetVolume(fVolume - m_fFadeValue);

		// 音量が0以下のとき破棄する
		if (fVolume <= 0.0f)
		{
			gameObject->Destroy();
		}
	}
}

//=============================================================
// [CSound] WAVEファイルを読み込む
//=============================================================
bool CSound::LoadWav(const std::string& sPath)
{
	// バッファを取得する
	ALuint bufferID = CDataManager::GetInstance()->RefAudio(sPath)->GetBuffer();

	// バッファをソースにセットする
	alSourcei(m_source, AL_BUFFER, bufferID);

	return true;
}

//=============================================================
// [CSound] 再生
//=============================================================
void CSound::Play()
{
	alSourcePlay(m_source);
}

//=============================================================
// [CSound] 停止
//=============================================================
void CSound::Stop()
{
	alSourceStop(m_source);
}

//=============================================================
// [CSound] 一時停止
//=============================================================
void CSound::Pause()
{
	alSourcePause(m_source);
}

//=============================================================
// [CSound] 巻き戻し
//=============================================================
void CSound::Rewind()
{
	alSourceRewind(m_source);
}

//=============================================================
// [CSound] ループ
//=============================================================
void CSound::SetLoop(const bool& bEnable)
{
	if (bEnable)
		alSourcei(m_source, AL_LOOPING, AL_TRUE);
	else
		alSourcei(m_source, AL_LOOPING, AL_FALSE);
}

//=============================================================
// [CSound] 音量
//=============================================================
void CSound::SetVolume(const float& fVolume)
{
	alSourcef(m_source, AL_GAIN, fVolume);
}

//=============================================================
// [CSound] ピッチ
//=============================================================
void CSound::SetPitch(const float& fPitch)
{
	alSourcef(m_source, AL_PITCH, fPitch);
}

//=============================================================
// [CSound] フェードアウト
//=============================================================
void CSound::FadeOut(const float& fFadeValue)
{
	m_fadeState = FADESTATE::FADEOUT;
	m_fFadeValue = fFadeValue;
}

//=============================================================
// [CSound] フェードイン
//=============================================================
void CSound::FadeIn(const float& fFadeValue, const float& fMaxVolume)
{
	m_fadeState = FADESTATE::FADEIN;
	m_fFadeValue = fFadeValue;
	m_fFadeMaxVolume = fMaxVolume;
}

//=============================================================
// [CSound] 状態の取得
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
// [CSound] 終了したらオブジェクトを破棄する
//=============================================================
void CSound::IsStoppedDestroy(const bool& bEnable)
{
	m_bStoppedDestroy = bEnable;
}


//=============================================================
// [CSoundListener] 初期化
//=============================================================
void CSoundListener::Init()
{
	// 前回の位置を今の位置に設定する
	m_oldPos = transform->GetWPos();
}

//=============================================================
// [CSoundListener] 終了
//=============================================================
void CSoundListener::Uninit()
{
	// リスナー設定を初期状態に戻す
	alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
	alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
	float fAngle = 0.0f;
	alListenerfv(AL_ORIENTATION, &fAngle);
}

//=============================================================
// [CSoundListener] 更新
//=============================================================
void CSoundListener::Update()
{
	// 位置を設定する
	ALfloat listenerPos[] = { transform->GetWPos().x, transform->GetWPos().y, transform->GetWPos().z };
	alListenerfv(AL_POSITION, listenerPos);

	// 方向を設定する
	if (m_bUpdateAngle)
	{
		CCamera* pCamera = gameObject->GetComponent<CCamera>();
		D3DXVECTOR3 posR = { 0.0f, 0.0f, 1.0f };

		if (pCamera != nullptr)
		{ // カメラがあるとき
			posR = pCamera->GetPosR() - transform->GetWPos();
			posR *= -1.0f;
		}
		else
		{
			D3DXMATRIX rotMtx = transform->GetRotationMatrix();
			D3DXVECTOR3 posR = { 0.0f, 0.0f, 1.0f };
			D3DXVec3TransformCoord(&posR, &posR, &rotMtx);
		}
		D3DXVec3Normalize(&posR, &posR);


		ALfloat listenerOri[] = { posR.x, posR.y, posR.z, 0.0f, 1.0f, 0.0f };
		alListenerfv(AL_ORIENTATION, listenerOri);
	}
}

//=============================================================
// [CSoundListener] 方向を設定する
//=============================================================
void CSoundListener::SetAngle(const float& fAngle)
{
	alListenerfv(AL_ORIENTATION, &fAngle);
}