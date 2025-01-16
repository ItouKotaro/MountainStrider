//=============================================================
//
// 竜巻イベント [tornado.h]
// Author: Ito Kotaro
//
//=============================================================
#ifndef _TORNADO_EVENT_H_
#define _TORNADO_EVENT_H_

#include "event_manager.h"

// 竜巻イベント
class TornadoEvent : public EventTemplate
{
public:
	void Init() override;
	void Uninit() override;
	void Update() override;

private:
	GameObject* m_tornade;

	float m_eventTimer;

	const float TORNADO_HEIGHT = 100.0f;
	const float EVENT_BASE_TIME = 50.0f;
	const float EVENT_RANDOM_TIME = 20.0f;
};

#endif // !_TORNADO_EVENT_H_
