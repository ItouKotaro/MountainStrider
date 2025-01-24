//=============================================================
//
// 環境 [environmental.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ENVIRONMENTAL_H_
#define _ENVIRONMENTAL_H_

#include "environmental_type.h"
#include "component/3d/camera.h"

// 環境効果
class EnvironmentalEffect
{
public:
	EnvironmentalEffect() : m_time(TIME::MORNING), m_weather(nullptr) {}
	void Init(const std::string path);
	void Uninit();
	void Update();
	CCamera* GetLightCamera() { return m_light->GetComponent<CCamera>(); }

	// 時間の種類
	enum TIME
	{
		MORNING,		// 朝
		DAYTIME,		// 昼
		NIGHT,			// 夜
		MAX
	};

	// 時間設定
	void SetTime(const TIME& time);

	// 天候設定
	void SetWeather(Weather* weather);
private:
	// ライト位置の更新
	void UpdateLight();

	TIME m_time;				// 時間
	Weather* m_weather;	// 天候
	GameObject* m_light;	// ライト（影用）
	GameObject* m_vehicle;// バイク（追跡用）

	AudioClip m_bgmClip;
	GameObject* m_bgm;	// BGM
};

#endif // !_ENVIRONMENTAL_H_
