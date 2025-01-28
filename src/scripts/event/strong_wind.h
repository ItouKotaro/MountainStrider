//=============================================================
//
// ã≠ïó [strong_wind.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _STRONG_WIND_EVENT_H_
#define _STRONG_WIND_EVENT_H_

#include "event_manager.h"

// Ë¶êŒÉCÉxÉìÉg
class StrongWindEvent : public EventTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;
private:
	GameObject* m_vehicle;
	float m_angle;
	float m_power;
	float m_eventTimer;

	const float MIN_POWER = 1000.0f;
	const float MAX_POWER = 2000.0f;
	const float EVENT_BASE_TIME = 10.0f;
	const float EVENT_RANDOM_TIME = 10.0f;
};

#endif // !_STRONG_WIND_EVENT_H_
