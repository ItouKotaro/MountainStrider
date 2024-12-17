//=============================================================
//
// ŠÂ‹« [environmental.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _ENVIRONMENTAL_H_
#define _ENVIRONMENTAL_H_

#include "environmental_type.h"

// ŠÂ‹«Œø‰Ê
class EnvironmentalEffect
{
public:
	EnvironmentalEffect() : m_time(TIME::MORNING), m_weather(nullptr) {}
	void Init();
	void Uninit();
	void Update();

	// ŠÔ‚Ìí—Ş
	enum TIME
	{
		MORNING,		// ’©
		DAYTIME,		// ’‹
		NIGHT,			// –é
		MAX
	};

	// ŠÔİ’è
	void SetTime(const TIME& time);

	// “VŒóİ’è
	void SetWeather(Weather* weather);
private:
	TIME m_time;				// ŠÔ
	Weather* m_weather;	// “VŒó
};

#endif // !_ENVIRONMENTAL_H_
