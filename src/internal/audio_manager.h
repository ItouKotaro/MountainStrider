//=============================================================
//
// オーディオ管理 [audio_manager.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

#include "component.h"

// オーディオ管理クラス
class CAudioManager
{
public:
	HRESULT Init();			// 初期化
	void Uninit();		// 終了
	void Update();	// 更新
private:
	ALCdevice* m_device;
	ALCcontext* m_context;
};

#endif // !_AUDIO_H_
